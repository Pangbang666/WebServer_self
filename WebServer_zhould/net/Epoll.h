//
// Created by zhoujc on 2020/6/8.
//
#pragma once

#include "../base/noncopyable.h"
#include <sys/epoll.h>
#include "Channel.h"
#include <memory>
#include <map>
#include <vector>
#include "../base/Timer.h"

class TimerManager;

class Epoll : noncopyable{
public:
    Epoll();
    ~Epoll();

    std::vector<std::shared_ptr<Channel>> poll();

    void epoll_add(std::shared_ptr<Channel> newChannel_, int timeout);
    void epoll_del(std::shared_ptr<Channel> delChannel_);
    void epoll_mod(std::shared_ptr<Channel> modChannel_, int timeout);

    void addTimer(std::shared_ptr<Channel> channel, int timeout);
    void handleExpired();
private:
    int epollFd_;
    std::vector<epoll_event> events_;
    std::map<int, std::shared_ptr<Channel>> fd2Channel_;
    std::map<int, std::shared_ptr<HttpData>> fd2HttpData_;
    TimerManager timerManager_;
};

