#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <sys/eventfd.h>
#include "TimeWheel.hpp"
#include "Channel.hpp"
#include "Poller.hpp"
#define DEFAULT_TIMEWHEEL_SIZE 1024
using namespace std;
using Func = function<void()>;
class Channel;
// reactor
// 1.事件监控 2.就绪事件处理 3.执行任务
class EventLoop
{
private:
    thread::id _id;
    Poller _poller;
    int _eventfd;
    unique_ptr<Channel> _eventfd_channel;
    mutex _mutex;
    vector<Func> _todo_list;
    vector<Channel *> _active_channels;
    TimeWheel _timewheel;

    bool SameThread()
    {
        return _id == this_thread::get_id();
    }
    void RunAllTask();
    int CreateEventfd();
    void ReadEventfd();
    // 为了防止在一个任务被放入todolist后没有新事件，epoll的wait阻塞导致任务得不到执行，添加eventfd并监听，在pushtodolist后执行下函数 使得wait不阻塞
    void WakeUpLoop();

public:
    EventLoop(int timewheel_size = DEFAULT_TIMEWHEEL_SIZE)
        : _id(this_thread::get_id()), _eventfd(CreateEventfd()),
          _eventfd_channel(new Channel(_eventfd, this,1)), _timewheel(timewheel_size, this) //,
    // _timerfd_channel(new Channel(_timewheel.Timerfd(),this))
    {
        AddChannel(_eventfd_channel.get()); // 添加用于唤醒epoll_wait的 eventfd监听
        _eventfd_channel->SetWritecb(bind(&EventLoop::WakeUpLoop, this));
        _eventfd_channel->EnableRead();
        AddChannel(_timewheel.GetChannelPtr()); // 添加timerfd监听；
        // lg(Debug,"timerfd:%d",_timewheel.GetChannelPtr()->Getfd());
        _timewheel.GetChannelPtr()->EnableRead();
    }
    void Start() // 应该死循环执行
        ;
    // 保证该loop管理的所有连接 都在该loop中执行任务 如果执行流不同 压入任务队列 随后在start中统一执行 保证线程安全
    void RunInLoop(Func f);

    void PushTodoList(Func f);
    void UpdateChannel(Channel *x);
    void RemoveChannel(Channel *x);
    void AddChannel(Channel *x);
    bool FindTimer(pair<int, int> id);
    //*可能有问题
    void RefreshTimerTaskInLoop(pair<int, int> id);
    void AddTimerTaskInLoop(pair<int, int> id, int delay, Func f);
    void CancelTimerTaskInLoop(pair<int, int> id);
};