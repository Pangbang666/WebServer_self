//
// Created by zhoujc on 2020/6/8.
//

#include "EventLoop.h"
#include <assert.h>

EventLoop::EventLoop()
    : pid_(0),
      poller_(){

}

EventLoop::~EventLoop() {

}

void EventLoop::loop() {
    assert(isInThread());
    while(true){
        std::vector<std::shared_ptr<Channel>> eventChannels_ = poller_.poll();
        for(size_t i = 0; i < eventChannels_.size(); i++){
            eventChannels_[i]->handleEvent();
        }
    }
}