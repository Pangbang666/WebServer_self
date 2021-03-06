//
// Created by zhoujc on 2020/5/7.
//
#pragma once
#include "noncopyable.h"
#include <pthread.h>
#include <functional>
#include <string>
#include "CountDownLatch.h"

class Thread :noncopyable {
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc& func, const std::string& name);
    ~Thread();
    void start();
    int join();
    bool started() const {return started_;}
    pid_t tid() const {return tid_;}
    std::string& name() {return name_;}
private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;
};


