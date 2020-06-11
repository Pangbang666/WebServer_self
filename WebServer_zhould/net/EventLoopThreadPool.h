//
// Created by zhoujc on 2020/6/10.
//
#pragma once
#include "../base/noncopyable.h"
#include "EventLoopThread.h"
#include <vector>

class EventLoopThreadPool : noncopyable{
public:
    explicit EventLoopThreadPool(size_t threadNum = 1);
    ~EventLoopThreadPool();

    void start();
    EventLoop* getNextLoop();
private:
    std::vector<std::shared_ptr<EventLoopThread>> threadPool_;
    std::vector<EventLoop*> loops_;
    size_t threadNum_;
    size_t nextLoopIndex_;
};

