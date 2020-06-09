//
// Created by zhoujc on 2020/6/8.
//
#pragma once

#include "../base/noncopyable.h"
#include "Poller.h"
#include <unistd.h>

class EventLoop : noncopyable{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    bool isInThread() { return getpid() == pid_; }

private:
    pid_t pid_;
    Poller poller_;
};

