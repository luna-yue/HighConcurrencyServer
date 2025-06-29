#pragma once
#include "EventLoop.hpp"
#include "LoopThread.hpp"
class LoopThreadPool{
   int _child_thread_cnt;
   EventLoop *_father_loop;
   vector<EventLoop*>_child_loop;
   vector<LoopThread*>_child_loopthread;
   int _next_idx;
   int LoadBalance()//可能需要用更高效的方法
   {
      _next_idx++;
      _next_idx%=_child_thread_cnt;
      return _next_idx;
   }
   public:
   LoopThreadPool(EventLoop * father_loop,int child_thread_cnt):_father_loop(father_loop),_child_thread_cnt(child_thread_cnt)
   {}
   void Create()
   {
    cout<<"_child_thread_cnt"<<_child_thread_cnt<<endl;
       if(_child_thread_cnt<0)
       {
        lg(Fatal,"LoopThreadPool::negative thread cnt%d",_child_thread_cnt);
        abort();
       }
       if(_child_thread_cnt>0)
       {
        _child_loopthread.resize(_child_thread_cnt);
        _child_loop.resize(_child_thread_cnt);
        _next_idx=0;
        for(int i=0;i<_child_thread_cnt;i++)
        {
            _child_loopthread[i]=new LoopThread();
            _child_loop[i]=_child_loopthread[i]->GetLoop();
        }
       }
   }

   EventLoop* NextLoop()
   {
     if(_child_thread_cnt==0)
     return _father_loop;
     return _child_loop[LoadBalance()];
   }
};