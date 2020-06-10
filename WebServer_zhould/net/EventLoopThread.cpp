//
// Created by zhoujc on 2020/6/10.
//

#include "EventLoopThread.h"
#include <functional>

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

}

EventLoop* EventLoopThread::start() {
    thread_.start();

    return loop_;
}

