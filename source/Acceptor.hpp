#pragma once
#include "Channel.hpp"
#include "Socket.hpp"
#include "EventLoop.hpp"
#include "Log.hpp"
#include <string>

using namespace std;
class Connection;
using ConnectionPtr = shared_ptr<Connection>;
class Acceptor
{
    Socket _socket;
    EventLoop *_loop;
    Channel _channel;
    using AcceptorCallback = function<void(int)>;
    AcceptorCallback _acceptorCallback; // 对新连接的处理回调函数 TcpServer设置 该函数中需要设置Connection中的多个回调函数
    int CreateServer(int port);
    void HandleRead();

public:
    Acceptor(EventLoop *loop, int port) : _loop(loop), _socket(CreateServer(port)), _channel(_socket.fd(), _loop,3)
    {
        _loop->AddChannel(&_channel);
        _channel.SetReadcb(bind(&Acceptor::HandleRead, this));
        // cout<<_channel.Getfd()<<endl;
        // cout<<"constructor done"<<endl;
    }
    void SetAcceptorcb(const AcceptorCallback &x);
};