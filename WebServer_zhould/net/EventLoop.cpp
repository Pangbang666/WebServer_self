//
// Created by zhoujc on 2020/6/8.
//

#include "EventLoop.h"
#include <assert.h>

EventLoop::EventLoop()
    : pid_(0),
      poller_(),
      isStartLoop_(false),
      timerManager_(){

}

EventLoop::~EventLoop() {

}

void EventLoop::addEvent(std::shared_ptr<Channel> channel_, size_t timeout) {
    poller_.addEvent(channel_);

    if(timeout > 0){
        timerManager_.addTimer(channel_, timeout);
    }
}

void EventLoop::modEvent(std::shared_ptr<Channel> channel_, size_t timeout) {
    poller_.modEvent(channel_);

    if(timeout > 0){
        timerManager_.addTimer(channel_,timeout);
    }
}

void EventLoop::delEvent(std::shared_ptr<Channel> channel_, size_t timeout) {
    poller_.delEvent(channel_);

    if(timeout > 0){
        timerManager_.addTimer(channel_, timeout);
    }
}

void EventLoop::loop() {
    assert(isInThread());
    assert(isStartLoop_);

    while(true){
        std::vector<std::shared_ptr<Channel>> eventChannels_ = poller_.poll();
        for(size_t i = 0; i < eventChannels_.size(); i++){
            eventChannels_[i]->handleEvent();
        }

        //超时处理
        std::vector<std::shared_ptr<Channel>> expiredChannels = timerManager_.tick();
    }
}