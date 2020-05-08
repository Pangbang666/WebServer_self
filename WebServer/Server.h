//
// Created by zhoujc on 2020/5/1.
//
#pragma once
#include <memory>
#include "EventLoop.h"
#include "Channel.h"
#include "EventLoopThreadPool.h"

class Server {
public:
    Server(EventLoop* loop, int threadNum, int port);
    ~Server() {};
    EventLoop* getLoop() { return loop_;}
    void start();
    void handleNewConn();
    void handleThisConn() { loop_->updatePoller(this->acceptChannel_);}
private:
    EventLoop* loop_;
    int threadNum_;
    std::unique_ptr<EventLoopThreadPool> eventLoopThreadPool_;
    bool started_;
    std::shared_ptr<Channel> acceptChannel_;
    int port_;
    int listenFd_;
    static  const int MAXFDS=10000;
};


