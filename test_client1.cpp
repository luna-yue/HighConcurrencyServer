//长连接测试
#include "../source/TcpServer.hpp"
int main()
{
    Socket x;
    x.CreateSocket();
    x.Connect("49.232.249.2",8101);
    int cnt=0;
    int i=0;
    string req="GET /GET HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\nsendcount: 0\r\n\r\n";
    while(1)
    {
        
        x.NonBlockSend((void*)req.c_str(),req.length());
        char res[65535];
        x.NonBlockRecv(res,65534);
        cout<<res<<endl;
        req="GET /GET HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\nsendcount: ";
        req+=to_string(++cnt);
        cout<<cnt<<endl;
        req+="\r\n\r\n";
        sleep(1);
    }
}