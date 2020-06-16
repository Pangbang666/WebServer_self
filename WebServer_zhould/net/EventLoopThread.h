//
// Created by zhoujc on 2020/6/10.
//
#pragma once
#include "../base/noncopyable.h"
#include "../base/Thread.h"
#include <memory>
#include <string>

class EventLoop;

class EventLoopThread {
public:
    explicit EventLoopThread(std::string name = "EventLoopThread");
    ~EventLoopThread();

    EventLoop* start();

private:
    void threadFunc();

private:
    EventLoop* loop_;
    Thread thread_;
    std::string name_;
    MutexLock mutex_;
    Condition cond_;
};

