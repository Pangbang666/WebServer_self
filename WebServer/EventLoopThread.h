//
// Created by zhoujc on 2020/5/8.
//
#pragma once
#include "EventLoop.h"
#include "base/noncopyable.h"
#include "base/Thread.h"
#include "base/MutexLock.h"
#include "base/Condition.h"

class EventLoopThread :noncopyable{
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop* startLoop();

private:
    void threadFunc();
    EventLoop* loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};

