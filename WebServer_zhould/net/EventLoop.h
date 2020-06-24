//
// Created by zhoujc on 2020/6/8.
//
#pragma once

#include "../base/noncopyable.h"
#include "Epoll.h"
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

    void addToPoller(std::shared_ptr<Channel> channel_, int timeout = 0);
    void updatePoller(std::shared_ptr<Channel> channel_, int timeout = 0);
    void delFromPoller(std::shared_ptr<Channel> channel_);

    bool isInThread() { return pid_ == CurrentThread::tid(); }
    bool isStartLoop() { return started_;}

    void wakeup();

private:
    void handleRead();

private:
    pid_t pid_;
    int wakeupFd_;
    Epoll poller_;
    bool started_;
    bool eventhandling_;
    std::shared_ptr<Channel> wakeupChannel_;

    MutexLock mutex_;
    bool callingPendingFunctors_;
    std::vector<Functor> pendingFunctors_;
};

