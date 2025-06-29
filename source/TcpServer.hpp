#pragma once
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "LoopThreadPool.hpp"
#include "Connection.hpp"
#include "Buffer.hpp"
#include <signal.h>
using namespace std;
#define DEFAULT_BUFFER_SIZE 1024
#define MAX_POLLER_SIZE 1024
#define DEFAULT_TIMEWHEEL_SIZE 1024
#define MAX_BUFFER_SIZE 1000000
#define DEFAULT_RELEASE_TIME 60
#define ll long long

class TcpServer
{
   using ConnectionPtr = shared_ptr<Connection>;
   EventLoop _base_loop;
   Acceptor _acceptor; // TcpServer所运行的线程（主线程）只负责accept并分发给从属线程
   LoopThreadPool _loop_thread_pool;
   bool _inactive_release; // 连接超时自动销毁
   int _release_time;
   int _next_conn_id;      // 用于给每个connection分配唯一的conn_id; 类似文件描述符
   int _port;
   unordered_map<int, ConnectionPtr> _all_conn; // 记录所有连接，调用CloseServerCallback 将连接从中删除（真正的）
   using ConnectionCallback = function<void(const ConnectionPtr &)>;
   using MessageCallback = function<void(const ConnectionPtr &, Buffer &)>;
   using AnyCallback = function<void(const ConnectionPtr &)>;
   using CloseCallback = function<void(const ConnectionPtr &)>;
   using CloseServerCallback = function<void(const ConnectionPtr &)>;
   ConnectionCallback _connectionCallback;
   MessageCallback _msgCallback;
   AnyCallback _anyCallback;
   CloseCallback _closeCallback;
   void EraseConnectionInLoop(const ConnectionPtr &x)
   {
      _all_conn.erase(x->ConnId());
   }
   void EraseConnection(const ConnectionPtr &x)
   {
      _base_loop.RunInLoop(bind(&TcpServer::EraseConnectionInLoop, this, x));
   }
   void HandleAccept(int fd);
   
public:
   TcpServer(int port, int thread_cnt)
       : _acceptor(&_base_loop, port),
         _loop_thread_pool(&_base_loop, thread_cnt),
         _inactive_release(false),
         _release_time(DEFAULT_RELEASE_TIME),
         _next_conn_id(1),
         _port(port)
   {
      _acceptor.SetAcceptorcb(bind(&TcpServer::HandleAccept, this, std::placeholders::_1)); // 添加新连接accept的回调并且调用EnableRead ，让baseloop监听acceptor的读事件
   }
   int GetPort() { return _port; }
   void SetConnectionCallback(const ConnectionCallback &x) { _connectionCallback = x; }
   void SetMessageCallback(const MessageCallback &x) { _msgCallback = x; }
   void SetCloseCallback(const CloseCallback &x) { _closeCallback = x; }
   void SetAnyCallback(const AnyCallback &x) { _anyCallback = x; }
   void EnableInactiveRelease(int time=DEFAULT_RELEASE_TIME){_inactive_release=true;_release_time=time;}
   void Start()
   {
      _loop_thread_pool.Create();
      _base_loop.Start();
   }
};
class NetWork
{
public:
   NetWork()
   {
      lg(Info, "SIGPIPE INIT");
      signal(SIGPIPE, SIG_IGN); // 在构造函数中忽略 SIGPIPE 向已经关闭的套接字写入会被收到该信号 导致程序终止 需要无视
   }
};
static NetWork nw;