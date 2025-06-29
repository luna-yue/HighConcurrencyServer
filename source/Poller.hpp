#pragma once
#include <unordered_map>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>
#include<vector>
#include <pthread.h>
#include "Log.hpp"
#include "Channel.hpp"
using namespace std;
#define MAX_POLLER_SIZE 1024
class Poller
{
    int _epfd;
    unordered_map<int, Channel *> _channels;
    epoll_event _revents_list[MAX_POLLER_SIZE];
    int Update(int fd, int op, epoll_event *event)
    {
        return epoll_ctl(_epfd, op, fd, event);
    }

public:
    Poller()
    {
        _epfd = epoll_create(1024);
        if (_epfd < 0)
        {
            lg(Fatal, "EPOLL CREATE FAILED %s", strerror(errno));
            abort();
        }
    }
    ~Poller()
    {
        if (_epfd > 0)
            close(_epfd);
    }
    void Remove(Channel *tmp);
    void Add(Channel *tmp);
    void Mod(Channel *tmp); // 传入想要关注的事件
    void Poll(vector<Channel *> &tmp);
};