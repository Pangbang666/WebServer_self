//
// Created by zhoujc on 2020/6/8.
//
#pragma once

#include "../base/noncopyable.h"
#include <functional>
#include "HttpData.h"

typedef std::function<void()> CallbackFunc;

class Channel : noncopyable{
public:
    Channel(int fd = 0);
    ~Channel();

    void handleEvent();

    int getFd();
    void setFd(int fd);
    void setEvents(__uint32_t events);
    void setReEvents(__uint32_t reEvents);
    __uint32_t getEvents();

    void setWriteCallback(const CallbackFunc& writeCallback) { writeCallback_ = writeCallback;}
    void setReadCallback(const CallbackFunc& readCallback) { readCallback_ = readCallback;}
private:
    int fd_;
    __uint32_t events_;
    __uint32_t reEvents_;

    HttpData httpData_;

    CallbackFunc readCallback_;
    CallbackFunc writeCallback_;
};

