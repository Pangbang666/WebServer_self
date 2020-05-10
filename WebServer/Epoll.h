//
// Created by zhoujc on 2020/5/4.
//
#pragma once
#include "Channel.h"
#include "HttpData.h"
#include <sys/epoll.h>
#include <vector>
#include <memory>
#include "Timer.h"

class Epoll {
public:
    typedef std::shared_ptr<Channel> SP_Channel;
    Epoll();
    ~Epoll();
    void epoll_add(SP_Channel request, int timeout);
    void epoll_mod(SP_Channel request, int timeout);
    void epoll_del(SP_Channel request);

    std::vector<SP_Channel> poll();
    std::vector<SP_Channel> getEventsRequest(int events_num);
    void add_timer(SP_Channel request_data, int timeout);
    int getEpollFd() { return epollFd_;}
    void handleExpired();

private:
    static const int MAXFDS=10000;
    int epollFd_;
    std::vector<epoll_event> events_;
    std::shared_ptr<Channel> fd2chan_[MAXFDS];
    std::shared_ptr<HttpData> fd2http_[MAXFDS];
    TimerManager timerManager_;
};


