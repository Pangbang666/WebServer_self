//
// Created by zhoujc on 2020/6/10.
//

#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(size_t threadNum) : threadNum_(threadNum){
    threadPool_ = std::vector<EventLoopThread>(threadNum, EventLoopThread());
}

EventLoopThreadPool::~EventLoopThreadPool() {

}

void EventLoopThreadPool::start() {
    for(size_t i = 0; i < threadNum_; i++){
        EventLoop* loop = threadPool_[i].start();
        loops_.push_back(loop);
    }
}