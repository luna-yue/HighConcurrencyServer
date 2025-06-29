#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <boost/container_hash/hash.hpp>
#include <sys/timerfd.h>
#include "Log.hpp"
#include "Channel.hpp"
//class Channel;
class EventLoop;
using namespace std;
using TaskFunc=function<void()>;
using ReleaseFunc=function<void()>;
//当引用计数减为0 则释放对象 在任务类析构函数中添加任务函数
class TimerTask{
  public:
   pair<int,int> _id;
   int _delay;
   ReleaseFunc _release;//在调用析构时 调用该函数来删除时间轮map中对该任务的记录
   TaskFunc _task_callbk;//要执行的任务
   bool _canceled;
   TimerTask(pair<int,int> id,int delay,TaskFunc task_callbk):_id(id),_delay(delay),_task_callbk(task_callbk),_canceled(false){}
   ~TimerTask()
   {
     if(!_canceled)
     _task_callbk();
     _release();
   }
   void SetRelease(const ReleaseFunc & func){
        _release=func;
   }
};
class TimeWheel{
  
    using WeakTask=weak_ptr<TimerTask>;
    using PtrTask=shared_ptr<TimerTask>;
    int _tick;
    int _capacity;//capacity指定 任务定时的最长时间
    unordered_map<pair<int,int>,WeakTask,boost::hash<pair<int,int>>> _timers;
    vector<vector<PtrTask>> _wheel;
    int _timerfd;
    EventLoop *_loop;
    unique_ptr<Channel> _timerfd_channel;//timerfd是存在TimeWheel中 用于保证时间轮正常轮转，时间每过一秒 则触发一次读事件，readcb则执行clock,执行定时任务
    void Delete(pair<int,int> id);
    public:
    TimeWheel(int capacity,EventLoop* loop):_capacity(capacity),_wheel(_capacity+3),_tick(0)
    ,_loop(loop),_timerfd(CreateTimerfd()),_timerfd_channel(new Channel(_timerfd,_loop,1))
    {
        _timerfd_channel->SetReadcb(bind(&TimeWheel::HandleTimerfdEvent,this));
    }
    int CreateTimerfd()
    ;
    void HandleTimerfdEvent()
    ;
    void Add(pair<int,int> id,int delay,TaskFunc work)
    ;
    void Refresh(pair<int,int> id)//对某项定时任务刷新执行时间
    ;
    void Clock()//应每秒执行一次 
    ;
    void Cancel(pair<int,int> id)//取消定时任务
    ;
    int NowTimers()
    {
      return _timers.size();
    }
    int Timerfd()
    {
        return _timerfd;
    }
    Channel* GetChannelPtr()
    {
        return _timerfd_channel.get();
    }
    bool FindTimer(pair<int,int> id)
    {
        return _timers.count(id);
    }
};