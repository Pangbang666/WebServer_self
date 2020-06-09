//
// Created by zhoujc on 2020/6/8.
//

#include "Channel.h"

Channel::Channel(int fd) : fd_(fd) {

}

Channel::~Channel() {

}

void Channel::handleEvent() {

}

int Channel::getFd() {
    return fd_;
}

__uint32_t Channel::getEvents() {
    return events_;
}