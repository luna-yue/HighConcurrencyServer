#include "TcpServer.hpp"
void TcpServer::HandleAccept(int fd)
   {
       ConnectionPtr new_conn=make_shared<Connection>(_loop_thread_pool.NextLoop(),fd,_next_conn_id++);
       _all_conn[new_conn->ConnId()]=new_conn;
       lg(Debug,"here got conn id & fd:> %d    %d",_next_conn_id-1,fd);
       if(_inactive_release)
       new_conn->EnableInactiveRelease(_release_time);
       if(_connectionCallback)new_conn->SetConnectionCallback(_connectionCallback);
       if(_msgCallback)new_conn->SetMessageCallback(_msgCallback);
       if(_anyCallback)new_conn->SetAnyCallback(_anyCallback);
       if(_closeCallback)new_conn->SetCloseCallback(_closeCallback);
       //给连接设置回调函数
       new_conn->SetCloseServerCallback(bind(&TcpServer::EraseConnection,this,std::placeholders::_1));
       new_conn->Establish();//这里用了EstablishInLoop 来保证是指定的子线程监听新连接
       
   }