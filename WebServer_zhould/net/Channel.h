//
// Created by zhoujc on 2020/6/8.
//
#pragma once

#include "../base/noncopyable.h"
#include <memory>
#include <functional>
#include "HttpData.h"

typedef std::function<void()> CallbackFunc;

class Channel{
public:
    Channel(EventLoop* loop, int fd);
    Channel(EventLoop* loop);
    ~Channel();

    void handleEvent();

    std::shared_ptr<HttpData> getHolder();
    void setHolder(std::shared_ptr<HttpData> holder);

    int getFd();
    void setFd(int fd);
    void setEvents(__uint32_t events);
    void setReEvents(__uint32_t reEvents);
    __uint32_t& getEvents();

    void setWriteCallback(const CallbackFunc& writeCallback) { writeCallback_ = writeCallback;}
    void setReadCallback(const CallbackFunc& readCallback) { readCallback_ = readCallback;}
private:
    int fd_;
    EventLoop* loop_;
    __uint32_t events_;
    __uint32_t reEvents_;

    std::weak_ptr<HttpData> holder_;

    CallbackFunc readCallback_;
    CallbackFunc writeCallback_;
};

