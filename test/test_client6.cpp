// 上传大文件测试
#include "../source/TcpServer.hpp"
#include "../Http/Utils.hpp"
int main()
{
    Socket x;
    x.CreateSocket();
    x.Connect("49.232.249.2",8101);
    int cnt=0;
    int i=0;
    
    string body;Utils::ReadFile("hello.txt",body);
    string req="PUT /1234.txt HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: ";
    req+=to_string(body.length());
    req+="\r\nsendcount: 0\r\n\r\n";
    req+=body;
        x.Send((void*)req.c_str(),req.length());
        sleep(10);
        char buf[1024];
        x.Recv(buf,1023);
        sleep(6000);
       
        
      
}