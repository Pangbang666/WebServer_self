//
// Created by zhoujc on 2020/5/4.
//
#pragma once
#include <memory>
#include "EventLoop.h"
#include <string>
#include <unistd.h>
#include "Timer.h"
#include "Channel.h"

using namespace std;

class HttpData: std::enable_shared_from_this<HttpData> {
public:
    HttpData(EventLoop *loop, int connfd);
    ~HttpData() { close(fd_);};
    void reset();
    void sepereteTimer();
    void linkTimer(std::shared_ptr<TimerNode> mtimer) {timer_=mtimer;}

    std::shared_ptr<Channel> getChannel() { return channel_;}
    EventLoop* getLoop() { return loop_;}
    void handleClose();
    void nowEvent();

private:
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
    int fd_;
    std::string inBuffer_;
    std::string outBuffer_;
    bool error_;

    std::weak_ptr<TimerNode> timer_;
};


