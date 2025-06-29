// 业务处理超时 当某个客户端的业务处理时间超过超时释放的时间 其他客户端连接应该被正确释放
#include "../source/TcpServer.hpp"
int main()
{

    string req = "GET /GET HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 0\r\n\r\n";
    for (int i = 1; i <= 10; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            Socket x;
            x.CreateSocket();
            x.Connect("49.232.249.2", 8101);
            x.NonBlockSend((void *)req.c_str(), req.length());
            return 0;
        }
    }
    while (1)
        ;
}