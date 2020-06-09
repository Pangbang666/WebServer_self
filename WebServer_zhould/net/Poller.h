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

class Poller : noncopyable{
public:
    Poller();
    ~Poller();

    std::vector<std::shared_ptr<Channel>> poll();

    void addEvent(std::shared_ptr<Channel> newChannel_);
    void delEvent(std::shared_ptr<Channel> delChannel_);
    void modEvent(std::shared_ptr<Channel> modChannel_);
private:
    int epollFd_;
    std::map<int,std::shared_ptr<Channel>> fd2Channel_;
};

