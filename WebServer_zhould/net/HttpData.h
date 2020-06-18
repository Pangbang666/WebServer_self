//
// Created by zhoujc on 2020/6/11.
//
#pragma once

#include "../base/noncopyable.h"
#include <string>
#include <memory>

class EventLoop;
class Channel;

class HttpData{
public:
    HttpData(EventLoop* loop, int fd);
    ~HttpData();

    void newEvent();

public:
    void readFd();
private:
    int fd_;
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
    std::string readBuffer_;
    std::string writeBuffer_;
};


