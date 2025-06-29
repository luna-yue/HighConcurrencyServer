#pragma once
#include <boost/any.hpp>
#include <functional>
using namespace std;
#include "EventLoop.hpp"
#include "Buffer.hpp"
#include "Socket.hpp"
class Connection : public std::enable_shared_from_this<Connection>{
   typedef enum {Disconnected,Disconnecting,Connected,Connecting}ConnStatus;
   using ConnectionPtr = shared_ptr<Connection>;
   using ConnectionCallback=function<void(ConnectionPtr&)>;
   using MessageCallback=function<void(ConnectionPtr&,Buffer &)>;
   using AnyCallback=function<void(ConnectionPtr&)>;
   using CloseCallback=function<void(ConnectionPtr&)>;
   using CloseServerCallback=function<void(ConnectionPtr&)>;
   EventLoop* _loop;
   Channel  _channel;
   ConnStatus status;
   Buffer _in_buffer;
   Buffer _out_buffer;
   ConnectionPtr self;
   int _conn_id;//connection的唯一标识
   int _timer_cnt;//该连接中定时任务的统计 与conn_id共同作为timer中查找的标识，1被设为定时销毁任务
   int _sockfd;
   Socket _socket;
   boost::any _context;
   ConnStatus _status;
   bool InactiveRelease;//是否非活跃超时销毁
   ConnectionCallback _connectionCallback; 
   MessageCallback _msgCallback;
   AnyCallback _anyCallback;
   CloseCallback _closeCallback;
   CloseServerCallback _closeserverCallback;
   void HandleResBuffer()
   ;
   void HandleRead();
   void HandleWrite();
   void HandleClose();
   void HandleError();
   void HandleAny();
   //并非直接释放 而是先处理缓冲区中的数据 可能发送给客户端数据
   void ShutDownInLoop();
   void ReleaseInLoop();
   void EstablishInLoop()
   ;
   void DisableInactiveReleaseInLoop()
   ;
   void EnableInactiveReleaseInLoop(int sec)
   ;
   void SendInLoop( Buffer & buf)
   ;
   public:
   Connection(EventLoop * loop,int sockfd,int conn_id)
   :_loop(loop),_channel(sockfd,loop,0),
   _conn_id(conn_id),_timer_cnt(2),_sockfd(sockfd),
   _socket(sockfd),_status(Connecting),InactiveRelease(false)
   {
    lg(Debug,"Connection construc:>%d",sockfd);
     _channel.SetReadcb(bind(&Connection::HandleRead,this));
     _channel.SetWritecb(bind(&Connection::HandleWrite,this));
     _channel.SetClosecb(bind(&Connection::HandleClose,this));
     _channel.SetErrorcb(bind(&Connection::HandleError,this));
     _channel.SetAnycb(bind(&Connection::HandleAny,this));
     _loop->AddChannel(&_channel);
   }
   
   ~Connection(){}
   void SetConnectionCallback(const  ConnectionCallback& x){_connectionCallback=x;}
   void SetMessageCallback(const  MessageCallback& x){_msgCallback=x;}
   void SetCloseCallback(const  CloseCallback& x){_closeCallback=x;}
   void SetAnyCallback(const  AnyCallback& x){_anyCallback=x;}
   void SetCloseServerCallback(const  CloseServerCallback& x){_closeserverCallback=x;}
   void SetContext(const boost::any & context){_context=context;}
   int ConnId()const{return _conn_id;}
   int AddTimerTask(int delay,Func task){
    _loop->AddTimerTaskInLoop(make_pair(_conn_id,_timer_cnt++),delay,task);
    return _timer_cnt-1;
   }
   void CancelTimerTask(int id){
      _loop->CancelTimerTaskInLoop(make_pair(_conn_id,id));
   }
   void DisableInactiveRelease()
   {
      _loop->RunInLoop(bind(&Connection::DisableInactiveReleaseInLoop,this));
   }
   void EnableInactiveRelease(int sec)
   {
     _loop->RunInLoop(bind(&Connection::EnableInactiveReleaseInLoop,this,sec));
   }
   void Establish()
   {
    _loop->RunInLoop(bind(&Connection::EstablishInLoop,this));
   }
   void ShutDown(){
     _loop->RunInLoop(bind(&Connection::ShutDownInLoop,this));
   }
   void Release(){
     _loop->PushTodoList(bind(&Connection::ReleaseInLoop,this));
   }
   void Send(const char * buf, int len)//向使用者提供的接口 并非直接发送 而是放入buffer 等待客户端fd读事件的发生
   {
     Buffer tmp;
     tmp.Write(buf,len);
     tmp.MoveWriteOffset(len);
     _loop->RunInLoop(bind(&Connection::SendInLoop,this,move(tmp)));
   }
   ConnStatus GetStatu(){return _status;}
   boost::any* GetContext(){return &_context;}
   void ChangeProtocol(){}

};