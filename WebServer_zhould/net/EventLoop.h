//
// Created by zhoujc on 2020/6/8.
//
#pragma once

#include "../base/noncopyable.h"
#include "Poller.h"
#include "../base/Timer.h"
#include "../base/MutexLock.h"
#include "../base/Condition.h"
#include "Channel.h"
#include <memory>
#include <unistd.h>
#include "../base/CurrentThread.h"

class EventLoop{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void addChannel(std::shared_ptr<Channel> channel_, size_t timeout = 0);
    void modChannel(std::shared_ptr<Channel> channel_, size_t timeout = 0);
    void delChannel(std::shared_ptr<Channel> channel_, size_t timeout = 0);

    void setPid(pid_t pid) { pid_ = pid;}
    pid_t getPid() { return pid_;}

    bool isInThread() { return pid_ == CurrentThread::tid(); }
    bool isStartLoop() { return isStartLoop_;}

private:
    pid_t pid_;
    int wakeupFd_;
    Poller poller_;
    bool isStartLoop_;
    TimerManager timerManager_;
    std::shared_ptr<Channel> wakeupChannel_;
};

