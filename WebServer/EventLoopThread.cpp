//
// Created by zhoujc on 2020/5/8.
//
#include "EventLoopThread.h"

EventLoopThread::EventLoopThread()
    : loop_(nullptr),
      exiting_(false),
      thread_(bind(&EventLoopThread::threadFunc, this), "EventLoopThread"),
      mutex_(),
      cond_(mutex_){};

EventLoopThread::~EventLoopThread() {
    exiting_=true;
    if(loop_){
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    assert(!thread_.started());
    thread_.start();
    {
        MutexLockGuard lock(mutex_);
        //一直等到threadFunc在thread_里面跑起来
        while(!loop_) cond_.wait();
    }

    return loop_;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        MutexLockGuard lock(mutex_);
        loop_=&loop;
        cond_.notify();
    }

    loop.loop();
    loop_= nullptr;
}