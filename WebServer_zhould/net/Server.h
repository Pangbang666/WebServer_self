//
// Created by zhoujc on 2020/6/11.
//
#pragma once

#include "../base/noncopyable.h"
#include "EventLoopThreadPool.h"
#include "Channel.h"
#include "EventLoop.h"
#include <memory>

class Server : noncopyable{
public:
    Server(EventLoop* loop, size_t threadNum, int port);
    ~Server();

    void start();

private:
    void acceptFunc();

private:
    int port_;
    int listenFd_;
    size_t threadNum_;
    EventLoop* loop_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;
    std::shared_ptr<Channel> acceptChannel_;
};

