//
// Created by zhoujc on 2020/6/10.
//
#pragma once
#include "../base/noncopyable.h"
#include "EventLoopThread.h"
#include <vector>

class EventLoopThreadPool : noncopyable{
public:
    EventLoopThreadPool(size_t threadNum = 1);
    ~EventLoopThreadPool();

    void start();
private:
    std::vector<EventLoopThread> threadPool_;
    std::vector<EventLoop*> loops_;
    size_t threadNum_;
};

