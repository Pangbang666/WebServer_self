//
// Created by zhoujc on 2020/6/8.
//

#include <assert.h>
#include <unistd.h>
#include "Epoll.h"
#include <vector>

#define MAXEVENTNUM 1000
#define TIMEOUT 1000

Epoll::Epoll()
    : epollFd_(epoll_create1(EPOLL_CLOEXEC)),
      events_(MAXEVENTNUM){
    assert(epollFd_ > 0);
}

Epoll::~Epoll() {
    close(epollFd_);
}

std::vector<std::shared_ptr<Channel>> Epoll::poll() {
    while(true) {
        int eventNum = epoll_wait(epollFd_, &*events_.begin(), events_.size(), TIMEOUT);
        if(eventNum > 0){
            std::vector<std::shared_ptr<Channel>> eventChannels_;
            for(size_t i = 0; i< eventNum; i++){
                int eventFd_ = events_[i].data.fd;
                if(fd2Channel_.find(eventFd_) != fd2Channel_.end()) {
                    std::shared_ptr<Channel> connChannel = fd2Channel_[eventFd_];
                    connChannel->setReEvents(events_[i].events);
                    eventChannels_.push_back(connChannel);
                }
            }
            return eventChannels_;
        }
    }
}

void Epoll::epoll_add(std::shared_ptr<Channel> newChannel_, int timeout) {
    int fd_ = newChannel_->getFd();
    __uint32_t events_ = newChannel_->getEvents();

    if(timeout > 0){
        std::shared_ptr<HttpData> holder_ = newChannel_->getHolder();
        if(holder_) {
            timerManager_.addTimer(newChannel_, timeout);
            fd2HttpData_[fd_] = holder_;
        }
    }

    fd2Channel_[fd_] = newChannel_;

    struct epoll_event event;
    event.events = events_;
    event.data.fd = fd_;
    epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd_, &event);

}

void Epoll::epoll_del(std::shared_ptr<Channel> delChannel_) {
    int fd_ = delChannel_->getFd();
    __uint32_t events_ = delChannel_->getEvents();

    if(fd2Channel_.find(fd_) == fd2Channel_.end())
        return;

    epoll_event event;
    event.events = events_;
    event.data.fd = fd_;
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd_, &event);

    fd2Channel_.erase(fd_);
    fd2HttpData_.erase(fd_);
}

void Epoll::epoll_mod(std::shared_ptr<Channel> modChannel_, int timeout) {
    int fd_ = modChannel_->getFd();
    __uint32_t events_ = modChannel_->getEvents();

    if(timeout > 0){
        timerManager_.addTimer(modChannel_, timeout);
    }

    assert(fd2Channel_.find(fd_) != fd2Channel_.end());

    epoll_event event;
    event.events = events_;
    event.data.fd = fd_;
    epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd_, &event);
}