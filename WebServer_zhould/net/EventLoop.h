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

class EventLoop : noncopyable{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void addEvent(std::shared_ptr<Channel> channel_, size_t timeout = 0);
    void modEvent(std::shared_ptr<Channel> channel_, size_t timeout = 0);
    void delEvent(std::shared_ptr<Channel> channel_, size_t timeout = 0);

    bool isInThread() { return getpid() == pid_; }
    bool isStartLoop() { return isStartLoop_;}

private:
    pid_t pid_;
    Poller poller_;
    bool isStartLoop_;
    TimerManager timerManager_;
};

