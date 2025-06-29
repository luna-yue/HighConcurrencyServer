#include "EventLoop.hpp"
void EventLoop::RunAllTask()
    {
        vector<Func> tmp;
        {
            unique_lock <mutex> lock(_mutex);
            tmp.swap(_todo_list);
        }
        for (auto &e : tmp)
            e();
        /*//可能省去之后todolist扩容消耗 但锁的范围更大
        {
          unique_lock lock(_mutex);
          for(auto &e:_todo_list)
          e();
          _todo_list.clear();
        }*/
    }
    int EventLoop::CreateEventfd()
    {
        int fd = eventfd(0, 0);
        if (fd < 0)
        {
            lg(Fatal, "EVENTLOOP:: EVENTFD CREATE ERROR %S", strerror(errno));
            abort();
        }
        return fd;
    }
    void EventLoop::ReadEventfd()
    {
       uint64_t res;
       res=eventfd_read(_eventfd,&res);
       if(res<0)
       {
         if(errno==EINTR||errno==EAGAIN)
         return;
         lg(Error, "ReadEventfd failed %s", strerror(errno));
       }
    }
    //为了防止在一个任务被放入todolist后没有新事件，epoll的wait阻塞导致任务得不到执行，添加eventfd并监听，在pushtodolist后执行下函数 使得wait不阻塞
    void EventLoop::WakeUpLoop()
    {
        int ret = eventfd_write(_eventfd, (uint64_t)1);
        if (ret < 0)
        {
            if(errno==EINTR)
            return;
            lg(Error, "WriteEventfd failed %s", strerror(errno));
        }
    }


    
    void EventLoop::Start()//应该死循环执行
    { // 1.事件监控
        lg(Info,"Poll Start");
        while(1){
        _active_channels.clear();
        _poller.Poll(_active_channels);
        
        //lg(Debug,"Poll finished");
        //lg(Debug,"_active_size %d",_active_channels.size());
        // 2.就绪事件处理
        for (auto &e : _active_channels)
        {
            //if(e->Getfd()>6)
            //cout<<"!!!!!"<<endl;
            e->HandleEvent();
        }
        // 3.执行任务
        RunAllTask();
        }
    }
    // 保证该loop管理的所有连接 都在该loop中执行任务 如果执行流不同 压入任务队列 随后在start中统一执行 保证线程安全
    void EventLoop::RunInLoop(Func f)
    {
        if (SameThread())
            f();
        else
        {
            PushTodoList(f);
            WakeUpLoop();
        }
    }
    
    void EventLoop::PushTodoList(Func f)
    {
        unique_lock<mutex> x(_mutex);
        _todo_list.push_back(f);
    }
    void EventLoop::UpdateChannel(Channel *x)
    {
        lg(Debug,"Update Channel %d",x->Getfd());
        _poller.Mod(x);
    }
    void EventLoop::RemoveChannel(Channel *x)
    {
        _poller.Remove(x);
    }
    void EventLoop::AddChannel(Channel *x)
    {
        _poller.Add(x);
    }
    bool EventLoop::FindTimer(pair<int,int>id)
    {
        return _timewheel.FindTimer(id);
    }
    //*可能有问题
    void EventLoop::RefreshTimerTaskInLoop(pair<int,int> id){
        RunInLoop(bind(&TimeWheel::Refresh,&_timewheel,id));
    }
    void EventLoop::AddTimerTaskInLoop(pair<int,int> id,int delay,Func f){
        RunInLoop(bind(&TimeWheel::Add,&_timewheel,id,delay,f));
    }
    void EventLoop::CancelTimerTaskInLoop(pair<int,int> id){
        RunInLoop(bind(&TimeWheel::Cancel,&_timewheel,id));
    }
void Channel::Update() { _loop->UpdateChannel(this); }
// 将channel从一个epoll中移除
void Channel::Remove() { _loop->RemoveChannel(this); }
void Channel::Add() { _loop->AddChannel(this); }