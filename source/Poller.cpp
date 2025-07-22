#include "Poller.hpp"
void Poller::Remove(Channel *tmp)
    {
        //cout<<"Remove channel"<<endl;
        int fd = tmp->Getfd();
        auto it = _channels.find(fd);
        if (it == _channels.end())
        {
            lg(Error, "Poller:: REMOVE NOT FOUND");
            return;
        }
        int ret = Update(fd, EPOLL_CTL_DEL, nullptr);
        //if (ret < 0)
        //{
         //   lg(Error, "Poller:: Remove ERROR %s", strerror(errno));
        //}
        _channels.erase(fd);
        close(fd);
    }
    void Poller::Add(Channel *tmp)
    {
        char threadName[16] = {0}; // Linux 限制线程名最长 16 字节（包括结尾 '\0'）
    pthread_getname_np(pthread_self(), threadName, sizeof(threadName));
    //cout<<string(threadName)<<"!";
        int fd = tmp->Getfd();
        lg(Debug,"POLLER::ADD RECEIVED%d",fd);
        auto it = _channels.find(fd);
        if (it != _channels.end())
        {
            lg(Warning, "Poller:: ADD EXISTED CHANNEL");
            return;
        }
        _channels[fd] = tmp;
        epoll_event events;
        events.data.fd = fd;
        events.events = tmp->GetEvents();
        int ret = Update(fd, EPOLL_CTL_ADD, &events);
        /*if (ret < 0)
        {
            //lg(Error, "Poller:: ADD ERROR %s", strerror(errno));
        }*/
        //cout<<"Add" <<tmp->Getfd()<<" "<<"done"<<endl;
    }
    void Poller::Mod(Channel *tmp) // 传入想要关注的事件
    {
        if(tmp==nullptr)
        {
            lg(Fatal,"null channel ptr !!");
        }
        int fd = tmp->Getfd();
        lg(Info,"Poller::Mod:>%d",fd);
        uint32_t new_events = tmp->GetEvents();
        lg(Info,"_channal size now:> %d",_channels.size());
        if (!_channels.count(fd))
            Add(tmp); // 如果不在_channel中先加入
        epoll_event events;
        events.data.fd = fd;
        events.events = new_events;
        int ret = Update(fd, EPOLL_CTL_MOD, &events);
        if (ret < 0)
        {
            //lg(Error, "Poller:: Mod ERROR %s", strerror(errno));
            return;
        }
        tmp->SetEvents(new_events);
    }

    void Poller::Poll(vector<Channel *> &tmp)
    {
        //lg(Debug,"Poll start");
        int n = epoll_wait(_epfd, _revents_list, MAX_POLLER_SIZE, -1);
        if (n == -1)
        {
            lg(Error, "epoll_wait error :>%s", strerror(errno));
            return;
        }
       // lg(Debug,"Poll recived:%d",n);
        for (int i = 0; i < n; i++)
        {
            int pos=_revents_list[i].data.fd;
            _channels[pos]->SetRevents(_revents_list[i].events);
            tmp.push_back(_channels[pos]);
        }
    }