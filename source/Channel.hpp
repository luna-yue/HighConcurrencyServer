#pragma once
#include <stdint.h>
#include <functional>
#include <sys/epoll.h>
#include <assert.h>
#include "Log.hpp"
using namespace std;
using Func = function<void()>;
class EventLoop;
enum ChannelType{
    ConnectionType=0,
    TimeWheelType,
    EventfdType,
    AcceptorType
};
class Channel
{
    using EventCallback = function<void()>;
    uint32_t _events;  // 该链接关心的事件
    uint32_t _revents; // 当前发生的事件
    int _fd;
    EventLoop *_loop;
    ChannelType _type;
    // 下为发生事件时的用来处理的回调函数
    EventCallback _readcb;
    EventCallback _writecb;
    EventCallback _closecb;
    EventCallback _errorcb;
    EventCallback _anycb; // 发生任意事件时的回调
public:
    Channel(int fd, EventLoop *loop,int type) : _events(EPOLLHUP|EPOLLERR), _revents(0), _fd(fd), _loop(loop),_type((ChannelType)type){}
    int Getfd()
    {
        return _fd;
    }
    void CheckLoop()
    {
        assert(_loop != nullptr);
    }
    bool ReadAble()
    {
        return _events&(0&EPOLLIN);
    }
    bool WriteAble()
    {
        return _events&(0&EPOLLOUT);
    }
    void EnableRead()
    {
        _events |= EPOLLIN;
        CheckLoop();
        lg(Debug,"EnableRead%d",_fd);
        Update();
    }
    void EnableWrite()
    {
        _events |= EPOLLOUT;
        CheckLoop();
        lg(Debug,"EnableWrite%d",_fd);
        Update();
    }
    void DisableRead()
    {
        _events &= ~EPOLLIN;
        CheckLoop();
        lg(Debug,"DisableRead%d!!!!:> %d",_fd,_type);
        Update();
    }
    void DisableWrite()
    {
        _events &= ~EPOLLOUT;
        CheckLoop();
        lg(Debug,"DisableWrite%d",_fd);
        Update();
    }
    uint32_t GetEvents()
    {
        return _events;
    }
    void SetLoop(EventLoop *loop)
    {
        _loop = loop;
    }
    void SetRevents(uint32_t revents)
    {
        _revents = revents;
    }
    void SetEvents(uint32_t x)
    {
        _events = x;
    }
    void SetReadcb(EventCallback x)
    {
        _readcb = x;
    }
    void SetWritecb(EventCallback x)
    {
        _writecb = x;
    }
    void SetClosecb(EventCallback x)
    {
        _closecb = x;
    }
    void SetErrorcb(EventCallback x)
    {
        _errorcb = x;
    }
    void SetAnycb(EventCallback x)
    {
        _anycb = x;
    }
    void HandleEvent();
    EventLoop* GetLoopPtr()
    {
        assert(_loop!=nullptr);
        return _loop;
    }
    // 将channel监控事件更新到一个epoll中
    void  Update();
    // 将channel从一个epoll中移除
    void  Remove();
    void  Add();
    
    
    
};