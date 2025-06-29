#include "Channel.hpp"
void Channel::HandleEvent()
{
    uint32_t tmp = _revents;
    if (tmp & EPOLLHUP)
    {
        
        if (_closecb != nullptr)
        {
            // cout<<"closehandle####"<<endl;
            _closecb();
            return;
        }
    }
    if (tmp & EPOLLERR)
    {

        
        if (_errorcb != nullptr)
        {
            // cout<<"errorhandle#####"<<endl;
            _errorcb();
            return;
        }
    }

    if (tmp & EPOLLIN)
    {
       // if(_fd>6)
        //lg(Info,"%d read event",_fd);
        
        if (_readcb != nullptr)
        {
            // cout<<"entered"<<endl;
            _readcb();
        }
    }
    if (tmp & EPOLLOUT)
    {
        // lg(Info,"write event");
        
        if (_writecb != nullptr)
        {
            // lg(Debug,"write cb");
            _writecb();
        }
    }
    if (_anycb != nullptr)
        {
            _anycb();
        }
}

