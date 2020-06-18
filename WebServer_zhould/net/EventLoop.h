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
    typedef std::function<void()> Functor;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void runInLoop(Functor&& cb);
    void queueInLoop(Functor&& cb);
    void doPendingFunctors();

    void addChannel(std::shared_ptr<Channel> channel_, size_t timeout = 0);
    void modChannel(std::shared_ptr<Channel> channel_, size_t timeout = 0);
    void delChannel(std::shared_ptr<Channel> channel_, size_t timeout = 0);

    bool isInThread() { return pid_ == CurrentThread::tid(); }
    bool isStartLoop() { return started_;}

    void wakeup();

private:
    void handleRead();

private:
    pid_t pid_;
    int wakeupFd_;
    Poller poller_;
    bool started_;
    bool eventhandling_;
    TimerManager timerManager_;
    std::shared_ptr<Channel> wakeupChannel_;

    MutexLock mutex_;
    bool callingPendingFunctors_;
    std::vector<Functor> pendingFunctors_;


};

