#include "TimeWheel.hpp"
void TimeWheel::Delete(pair<int,int> id)
    {
       auto it=_timers.find(id);
       if(it==_timers.end())
       return;
       _timers.erase(id);
       lg(Debug,"Delete %d done",id);
    }
    
    
    int TimeWheel::CreateTimerfd()
    {
       _timerfd=timerfd_create(CLOCK_REALTIME,0);
        if(_timerfd<0)
        {
            lg(Fatal,"timerfd create failed");
            abort();
        }
        struct itimerspec new_value;
        new_value.it_interval.tv_sec=(time_t)1;
        new_value.it_interval.tv_nsec=(long long)0;
        new_value.it_value.tv_sec=(time_t)1;
        new_value.it_value.tv_nsec=(long long)0;
        timerfd_settime(_timerfd,0,&new_value,nullptr);
        return _timerfd;
    }
    void TimeWheel::HandleTimerfdEvent()
    {
        uint64_t n;
        int res=read(_timerfd,&n,sizeof(uint64_t));
        if(res<0)
        {
            lg(Error,"Timerfd read error%s",strerror(errno));
            abort();
        }
        for(uint64_t i=1;i<=n;i++)
        {
           Clock();
        }
    }
    void TimeWheel::Add(pair<int,int> id,int delay,TaskFunc work)
    {
         lg(Debug,"TimeWheel::Add:>%d %d",id.first,id.first);
          PtrTask tmp=make_shared<TimerTask>(id,delay,work);
          //cout<<_capacity<<endl;
          auto it=_timers.find(id);
          if(it!=_timers.end())
          {
           lg(Warning,"TimeWheel:: Add failed id already used");
           return;
          }
          _timers[id]=WeakTask(tmp);  //这里哈希表存weakptr可行原因是_wheel中至少存在一个shared_ptr
          int pos=_tick+tmp->_delay;
          //cout<<pos<<" "<<_capacity<<endl;

          pos%=_capacity; 
          _wheel[pos].push_back(tmp);
          tmp->SetRelease(bind(&TimeWheel::Delete,this,id));
    } 
    void TimeWheel::Refresh(pair<int,int> id)//对某项定时任务刷新执行时间
    {
      lg(Debug,"TimeWheel::Refresh:>%d %d",id.first,id.first);
      if(_timers.find(id)==_timers.end())
      {
        lg(Debug,"TimeWheel:: refresh not exist");
        return;
      }
      WeakTask x=_timers[id];
      PtrTask xx=x.lock();
      int pos=_tick+xx->_delay;
      lg(Debug,"tick::> %d delay:> %d",_tick,xx->_delay);
      pos%=_capacity; 
      _wheel[pos].push_back(xx);//anycallback 都会导致refresh 可能重复放入同一定时任务
    }
    void TimeWheel::Clock()//应每秒执行一次 
    {
      
      _tick++;
      _tick%=_capacity;
      //lg(Debug,"clock sz:>%d",_wheel[_tick].size());
      if(_wheel[_tick].size()!=0)
      {
        lg(Debug,"here timework start ,sz:> %d",_wheel[_tick].size());
      }
      _wheel[_tick].clear();
    }
    void TimeWheel::Cancel(pair<int,int> id)//取消定时任务
    {
      auto it=_timers.find(id);
      if(it==_timers.end())
      {
        //lg(Warning,"TimeWheel:: Cancel failed notfound");
        return;
      }
      auto ptr=it->second.lock();
      ptr->_canceled=true;
    }