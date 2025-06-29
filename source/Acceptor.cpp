#include "Acceptor.hpp"
int Acceptor::CreateServer(int port)
{
    _socket.CreateSocket();
    _socket.ReUseAddr();//设置端口复用
    _socket.Bind(port);
    _socket.Listen();
    lg(Info,"Create Server done");
    return _socket.fd();
}
void Acceptor::HandleRead()
{
    // cout<<"read"<<endl;
    string ip;
    uint16_t port;
    int fd = _socket.Accept(&ip,&port);
    if (fd < 0)
        return;
    // cout<<fd<<endl;
    lg(Debug,"Acceptor::Accept from>%s %d",ip.c_str() ,port);
    assert(_acceptorCallback != nullptr);
    _acceptorCallback(fd);
}
void Acceptor::SetAcceptorcb(const AcceptorCallback &x)
{
    _acceptorCallback = x;
    _channel.EnableRead(); // 必须完成设置回调后才能启动读事件的监控 否则新连接到来 得不到正确处理 资源泄露
}