//
// Created by zhoujc on 2020/6/11.
//

#include <unistd.h>
#include "HttpData.h"
#include "EventLoop.h"

#define MAXSIZE 10240

const __uint32_t DEFAULT_EVENTS = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
const int DEFAULT_EXPIRED_TIME = 2000;
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000;

HttpData::HttpData(EventLoop* loop, int fd)
    : fd_(fd),
      loop_(loop),
      channel_(new Channel(loop_, fd_)),
      readBuffer_(std::string()),
      writeBuffer_(std::string()){

}

HttpData::~HttpData() {
}

void HttpData::readFd() {
    readBuffer_.clear();
    char buffer[MAXSIZE];
    ssize_t len = read(fd_, (void*)buffer, MAXSIZE);
}

void HttpData::newEvent() {
    channel_->setEvents(DEFAULT_EVENTS);
    loop_->addChannel(channel_, DEFAULT_EXPIRED_TIME);
}