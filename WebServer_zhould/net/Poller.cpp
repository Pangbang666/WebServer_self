//
// Created by zhoujc on 2020/6/8.
//

#include <assert.h>
#include <unistd.h>
#include "Poller.h"
#include <vector>

#define MAXEVENTNUM 1000
#define TIMEOUT 1000

Poller::Poller()
    : epollFd_(epoll_create1(EPOLL_CLOEXEC)),
      events_(MAXEVENTNUM){
    assert(epollFd_ > 0);
}

Poller::~Poller() {
    close(epollFd_);
}

std::vector<std::shared_ptr<Channel>> Poller::poll() {
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

void Poller::addEvent(std::shared_ptr<Channel> newChannel_) {
    int fd_ = newChannel_->getFd();
    __uint32_t events_ = newChannel_->getEvents();

    fd2Channel_[fd_] = newChannel_;

    struct epoll_event event;
    event.events = events_;
    event.data.fd = fd_;
    epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd_, &event);
}

void Poller::delEvent(std::shared_ptr<Channel> delChannel_) {
    int fd_ = delChannel_->getFd();
    __uint32_t events_ = delChannel_->getEvents();

    if(fd2Channel_.find(fd_) == fd2Channel_.end())
        return;

    epoll_event event;
    event.events = events_;
    event.data.fd = fd_;
    epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd_, &event);

    fd2Channel_.erase(fd_);
}

void Poller::modEvent(std::shared_ptr<Channel> modChannel_) {
    int fd_ = modChannel_->getFd();
    __uint32_t events_ = modChannel_->getEvents();

    assert(fd2Channel_.find(fd_) != fd2Channel_.end());

    epoll_event event;
    event.events = events_;
    event.data.fd = fd_;
    epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd_, &event);
}