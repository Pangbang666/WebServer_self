//
// Created by zhoujc on 2020/6/8.
//

#include <sys/epoll.h>
#include "Channel.h"

Channel::Channel(int fd)
    : fd_(fd),
      events_(0),
      reEvents_(0),
      httpData_(fd),
      readCallback_(std::bind(&HttpData::readFd, &httpData_)){
}

Channel::~Channel() {

}

void Channel::handleEvent() {
    if(reEvents_ & EPOLLIN){
        readCallback_();
    }else if(reEvents_ & EPOLLOUT){
        writeCallback_();
    }

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

__uint32_t Channel::getEvents() {
    return events_;
}