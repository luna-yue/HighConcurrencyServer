//重复发送不完整数据 数据得不到处理 那么最后客户端应该超时关闭
#include "../source/TcpServer.hpp"
int main()
{
    Socket x;
    x.CreateSocket();
    x.Connect("49.232.249.2",8101);
    string req="GET /GET HTTP/1.1\r\nConnection: keep-alive\r\nContent-Length: 100\r\n\r\nhello";
    while(1){
        
        assert(x.NonBlockSend((void*)req.c_str(),req.length()) != -1);
        assert(x.NonBlockSend((void*)req.c_str(),req.length()) != -1);
        assert(x.NonBlockSend((void*)req.c_str(),req.length()) != -1);
        char buf[10240] = {0};
        sleep(3);
        assert(x.Recv(buf, 10239));
        lg(Debug,"[%s]", buf);
        
    }
}