//
// Created by zhoujc on 2020/6/8.
//
#pragma once

#include "../base/noncopyable.h"
#include <functional>

typedef std::function<void()> CallbackFunc;

class Channel : noncopyable{
public:
    Channel(int fd);
    ~Channel();

    void handleEvent();

    int getFd();
    __uint32_t getEvents();

    void setWriteCallback(const CallbackFunc& writeCallback) { writeCallback_ = writeCallback;}
    void setReadCallback(const CallbackFunc& readCallback) { readCallback_ = readCallback;}
private:
    int fd_;
    __uint32_t events_;
    CallbackFunc readCallback_;
    CallbackFunc writeCallback_;
};

