//
// Created by zhoujc on 2020/5/8.
//
#pragma once
#include "base/noncopyable.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include <vector>

class EventLoopThreadPool :noncopyable{
public:
    EventLoopThreadPool(EventLoop* baseLoop,int numThreads);
    ~EventLoopThreadPool() { LOG<<"~EventLoopTHreadPool";}
    void start();

    EventLoop* getNextLoop();

private:
    EventLoop* baseLoop_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};


