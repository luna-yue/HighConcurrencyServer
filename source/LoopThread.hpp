#pragma once
#include "EventLoop.hpp"
#include <condition_variable>
class LoopThread{
   mutex _mutex;
   condition_variable _cond;
   EventLoop* _loop;
   thread _thread;
   void RunThread()
   {   EventLoop x;
       {
        unique_lock <mutex> lock(_mutex);
        _loop=&x;
        _cond.notify_all();
       }
       x.Start();
   }
   public:
   LoopThread():_loop(nullptr),_thread(&LoopThread::RunThread,this)
   {}
   EventLoop* GetLoop()
   {
     unique_lock <mutex> lock(_mutex);
     _cond.wait(lock,[&](){return _loop!=nullptr;});
     return _loop;
   }
};