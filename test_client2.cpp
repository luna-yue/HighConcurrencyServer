//超时销毁测试
#include "../source/TcpServer.hpp"
int main()
{
    Socket x;
    x.CreateSocket();
    x.Connect("49.232.249.2",8101);
    int cnt=0;
    string req="GET /GET HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\nsendcount: 0\r\n\r\n";
    
        x.NonBlockSend((void*)req.c_str(),req.length());
        char res[65535];
        
        x.NonBlockRecv(res,65534);
        lg(Debug,"first send done");
        sleep(1);
        while(1){
        int ret=x.Recv(res,65534);
        lg(Debug,"recv result:>%d",ret);
        if(ret==-1)
        return 0;
        sleep(1);
        }
    
}