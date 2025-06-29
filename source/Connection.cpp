#include "Connection.hpp"
void Connection::HandleResBuffer()
{
    if (_in_buffer.empty())
        return;
    if (_msgCallback)
        _msgCallback(self, _in_buffer);
}
void Connection::HandleRead()
{
    if(_sockfd>6)
    lg(Info,"Connection Handle read %d",_sockfd);
    char buf[65535];
    int ret = _socket.NonBlockRecv(buf, 65534);
    //lg(Debug,"Read Get:> %s",buf);
    if (ret < 0)
    {
        lg(Debug,"Connction:: read result:>%d",ret);
        // 可能还有数据待发送 待处理 不能直接release
        ShutDownInLoop();
    }
    else
    {
        _in_buffer.Write(buf, ret);
        _in_buffer.MoveWriteOffset(ret);
        if (_msgCallback)
            _msgCallback(self, _in_buffer);
    }
}
void Connection::HandleWrite()
{
    lg(Debug,"Connection::>handle write start");
    lg(Debug,"Connection statu:>%d",_status);
    int cnt=0;
    while(!_out_buffer.empty()){
    char buf[65535];
    uint64_t sz=65535;
    uint64_t res=min(sz,_out_buffer.UsedSpace());
    _out_buffer.Read(buf,res );
    int n = _socket.NonBlockSend(buf, res);
    lg(Debug,"write::read buf result:>%d",n);
    if (n < 0)
    {
        HandleResBuffer();
        return Release();
    }
    else
    {
        _out_buffer.MoveReadOffset(res);
    }
    }
        _channel.DisableWrite();
        if(_status==Disconnecting){
            lg(Debug,"status==disconnecting gonna release");
        return Release();}
}
void Connection::HandleClose()
{
    HandleResBuffer();
    Release();
}
void Connection::HandleError()
{
    HandleResBuffer();
    Release();
}
void Connection::HandleAny()
{
    if (InactiveRelease)
        _loop->RefreshTimerTaskInLoop({_conn_id, 1});
    if (_anyCallback)
        _anyCallback(self);
}
// 并非直接释放 而是先处理缓冲区中的数据 可能发送给客户端数据
void Connection::ShutDownInLoop()
{
    lg(Debug,"Shutdown in loop start %d",_conn_id);
    _status=Disconnecting;
    if (!_in_buffer.empty() && _msgCallback)
        _msgCallback(self, _in_buffer);
    if (!_out_buffer.empty())
        _channel.EnableWrite();
    if (_out_buffer.empty())
        Release();
}
void Connection::ReleaseInLoop()
{
    
    _status=Disconnected;
    lg(Debug,"Release:>%d",_sockfd);
    _loop->RemoveChannel(&_channel);
    close(_sockfd);
    
    // 取消所有定时任务
    pair<int, int> tmp(_conn_id, 1);
    if (InactiveRelease)
        _loop->CancelTimerTaskInLoop(tmp);
    for (int i = 2; i < _timer_cnt; i++)
    {
        _loop->CancelTimerTaskInLoop(tmp);
        tmp.second++;
    }
    if (_closeCallback)
        _closeCallback(self);
    _closeserverCallback(self);
    
}
void Connection::EstablishInLoop()
{
    // cout<<"establish start"<<endl;
     assert(_status==Connecting);
    _status = Connected;
    _channel.EnableRead();
    self = shared_from_this();
    if (_connectionCallback)
        _connectionCallback(self);
    // else cout<<"conn cb null \n";
    // cout<<"establish done"<<endl;
}
void Connection::DisableInactiveReleaseInLoop()
{
    InactiveRelease = false;
    pair<int, int> tmp(_conn_id, 1);
    if (!(_loop->FindTimer(tmp)))
        return;
    _loop->CancelTimerTaskInLoop(tmp);
}
void Connection::EnableInactiveReleaseInLoop(int sec)
{
    InactiveRelease = true;
    pair<int, int> tmp(_conn_id, 1);
    if (_loop->FindTimer(tmp))
     return  _loop->RefreshTimerTaskInLoop(tmp);
    _loop->AddTimerTaskInLoop(tmp,sec,bind(&Connection::Release,this));
}
void Connection::SendInLoop(Buffer &buf)
{
    _out_buffer.Write(buf);
    int sz=buf.UsedSpace();
    _out_buffer.MoveWriteOffset(sz);
    cout<<"send in loop :> out buffer used:>"<<_out_buffer.UsedSpace()<<endl<<endl;
    if (!_channel.WriteAble())
        _channel.EnableWrite();
}