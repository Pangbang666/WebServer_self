//
// Created by zhoujc on 2020/5/4.
//
#pragma once
#include <functional>
#include <memory>

class EventLoop;
class HttpData;

class Channel {
private:
    typedef std::function<void()> callback;
    EventLoop *loop_;
    int fds_;
    __uint32_t events_;
    __uint32_t revents_;
    __uint32_t lastEvents_;

    //方便找到上层持有该channel的对象
    std::weak_ptr<HttpData> holder_;

private:
    int parse_URI();
    int parse_Headers();
    int analysisRequest();

    callback readHandler_;
    callback writeHandler_;
    callback errorHandler_;
    callback connHandler_;

public:
    Channel(EventLoop* loop);
    Channel(EventLoop* loop, int fd);
    ~Channel();

    int getFd();
    void setFd(int fd);

    void setHolder(std::shared_ptr<HttpData> holder) { holder_=holder;}
    std::shared_ptr<HttpData> getHolder() {
        std::shared_ptr<HttpData> ret(holder_.lock());
        return ret;
    }

    void setReadHandler(callback&& readHandler) { readHandler_=readHandler;}
    void setWriteHandler(callback&& writeHandler) { writeHandler_=writeHandler;}
    void setErrorHandler(callback&& errorHandler) { errorHandler_=errorHandler;}
    void setConnHandler(callback&& connHandler) { connHandler_=connHandler;}
};


