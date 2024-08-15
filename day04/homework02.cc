#include <iostream>
#include <string>

#include <signal.h>
//to use fstat
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

#include <workflow/WFFacilities.h>
#include <workflow/WFHttpServer.h>

using std::cout;
using std::string;
using std::cerr;

static WFFacilities::WaitGroup waitGroup(1);

void signalHandler(int singNum){
    cout<<"End!\n";
    waitGroup.done();
}

//保存信息，使得服务器端可以读取信息向redis确认信息
//并将对比得到的信息返回给客户端
struct SeriesContext{
    string name;
    string password;
    protocol::HttpResponse* resp;
};

//服务的回调函数
void process(WFHttpTask* serverTask){
    //创建httprequest任务，这个是客户端发来的请求
    protocol::HttpRequest* req = serverTask->get_req();
    //回复给客户端的响应
    protocol::HttpResponse* resp = serverTask->get_resp();
    //获取方法，确定下一步动作
    string method = req->get_method();
    //判断方法类型
    if(method == "GET"){
        
        int fd = open("postform.html",O_RDWR);
        if(!fd){
            error(1,errno,"open failed in process");
        }
        
        struct stat filestat;
        if (fstat(fd, &filestat) < 0) {
            perror("fstat failed");
            close(fd);
        }
        
        size_t lenth = filestat.st_size;
        char* buf = new char[lenth + 1]{0};
        size_t readlenth = read(fd,buf,lenth);

        resp->append_output_body(buf,readlenth);
        delete [] buf;

    }else if(method == "POST"){
        
        const void* body;
        size_t size;
        req->get_parsed_body(&body,&size);

        string bodystr((const char*)body);
        //以&符号进行切割
        //“name”=“qinhao”&"password"="123456" 
        string nameKV = bodystr.substr(0,bodystr.find("&"));
        string passwordKV = bodystr.substr(bodystr.find("&")+1);

        string name = nameKV.substr(nameKV.find("=")+1);
        string password = passwordKV.substr(passwordKV.find("=")+1);

        SeriesContext* context = new SeriesContext;
        context->name = name;
        context->password = password;
        context->resp = resp;

        series_of(serverTask)->set_context(context);
        //创建redis任务进行查询
        WFRedisTask* redisTask = WFTaskFactory::create_redis_task(
            "redis://127.0.0.1:6379",
             10,
            [=](WFRedisTask* redisTask){
                protocol::RedisResponse* respRedis = redisTask->get_resp();
                protocol::RedisValue result;
                respRedis->get_result(result);
    //            SeriesContext *context = (SeriesContext*)(series_of(redisTask)->get_context());
                    
                if(result.is_string() && result.string_value() == context->password){
                // 在redisTask的回调中去修改serverTask的响应
                context->resp->append_output_body("<html>Success</html>");
                }else{
                context->resp->append_output_body("<html>Fail</html>");
                }
                //delete context;         
            }
                                                                );
        redisTask->get_req()->set_request("HGET",{"user",name});
        series_of(serverTask)->push_back(redisTask);
        //delete context;
    }

}

int main(){
    
    signal(SIGINT,signalHandler);
    //创建一个服务
    WFHttpServer server(process);
    //判断是否启动成功
    if(server.start(61200) != 0){
        
        perror("server start fail!");
        //减少一个次数，准备退出
        exit(-1);

    }else{
        waitGroup.wait();
        cout<<"exit.....\n";
        server.stop();
        return 0;
    }
}
