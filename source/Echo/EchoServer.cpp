#include "../TcpServer.hpp"
class EchoServer{
  private:
   TcpServer _server;
  private:
   void Msgcb(const ConnectionPtr &tmp,Buffer & buf){
    char  tmpbuf[65535];
    cout<<"Readable sz "<<buf.UsedSpace()<<endl;
    int sz=min((int)buf.UsedSpace(),65535);
    cout<<"sz"<<sz<<endl;
    buf.Read(tmpbuf,sz);
    buf.MoveReadOffset(sz);

    string ans(tmpbuf);
    cout<<"Received ans:"<<ans<<endl;
    tmp->Send(ans.c_str(),ans.size());
    cout<<"before shutdown"<<" "<<buf.UsedSpace()<<" "<<buf.empty()<<endl;
    tmp->ShutDown();
    }
    void Newcb(const ConnectionPtr &tmp)
    {
      lg(Info,"New connection%p",tmp.get());
    }
    void Closecb(const ConnectionPtr &tmp)
    {
      lg(Info,"Connection %p close",tmp.get());
    }
  public:
    EchoServer(int port,int thread_cnt):_server(port,thread_cnt)
    {
      _server.SetConnectionCallback(std::bind(&EchoServer::Newcb,this,placeholders::_1));
      _server.SetMessageCallback(std::bind(&EchoServer::Msgcb,this,placeholders::_1,placeholders::_2));
      _server.SetCloseCallback(std::bind(&EchoServer::Closecb,this,placeholders::_1));
    }
    void Start()
    {
      _server.Start();
    }
};