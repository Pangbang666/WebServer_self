//
// Created by zhoujc on 2020/6/10.
//

#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(size_t threadNum)
    : threadNum_(threadNum),
      nextLoopIndex_(0){
    for(size_t i = 0; i < threadNum; i++){
        std::string threadName = "thread_" + std::to_string(i);
        std::shared_ptr<EventLoopThread> thread(new EventLoopThread(threadName));
        threadPool_.push_back((thread));
    }
}

EventLoopThreadPool::~EventLoopThreadPool() {

}

void EventLoopThreadPool::start() {
    for(size_t i = 0; i < threadNum_; i++){
        EventLoop* loop = threadPool_[i]->start();
        loops_.push_back(loop);
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    EventLoop* loop = loops_[nextLoopIndex_];
    nextLoopIndex_ = (nextLoopIndex_ + 1)%threadNum_;
    return loop;
}