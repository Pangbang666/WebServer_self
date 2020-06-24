//
// Created by zhoujc on 2020/6/8.
//

#include <sys/epoll.h>
#include "Channel.h"

Channel::Channel(EventLoop* loop, int fd)
    : fd_(fd),
      loop_(loop),
      events_(0),
      reEvents_(0){
}

Channel::Channel(EventLoop *loop)
    : loop_(loop),
      events_(0),
      reEvents_(0){

}

Channel::~Channel() {

}

void Channel::handleEvent() {
    if(reEvents_ & EPOLLIN){
        readCallback_();
    }else if(reEvents_ & EPOLLOUT){
        writeCallback_();
    }

    setReEvents(0);
}

std::shared_ptr<HttpData> Channel::getHolder() {
    std::shared_ptr<HttpData> ret(holder_.lock());
    return ret;
}

void Channel::setHolder(std::shared_ptr<HttpData> holder) {
    holder_ = holder;
}

int Channel::getFd() {
    return fd_;
}

void Channel::setFd(int fd) {
    fd_ = fd;
}

void Channel::setEvents(__uint32_t events) {
    events_ = events;
}

void Channel::setReEvents(__uint32_t reEvents) {
    reEvents_ = reEvents;
}

__uint32_t& Channel::getEvents() {
    return events_;
}