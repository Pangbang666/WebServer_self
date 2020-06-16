//
// Created by zhoujc on 2020/6/10.
//

#include "EventLoopThread.h"
#include <functional>
#include <assert.h>
#include "EventLoop.h"

EventLoopThread::EventLoopThread(std::string name)
    : name_(name),
      loop_(nullptr),
      mutex_(),
      cond_(mutex_),
      thread_(std::bind(&EventLoopThread::threadFunc, this), name){
}

EventLoopThread::~EventLoopThread() {

}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        MutexLockGuard mutexGuard_(mutex_);
        loop_ = &loop;
        cond_.notify();
    }

    loop.loop();

    loop_ = nullptr;
}

EventLoop* EventLoopThread::start() {
    assert(!thread_.isStarted());
    thread_.start();
    {
        MutexLockGuard mutexGuard_(mutex_);
        while (!loop_) {
            cond_.wait();
        }
    }

    return loop_;
}

