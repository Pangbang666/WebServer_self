//
// Created by zhoujc on 2020/6/4.
//
#pragma once
#include "noncopyable.h"
#include <pthread.h>
#include <functional>
#include <string>
#include "Condition.h"

typedef std::function<void()> ThreadFunc;

class Thread : noncopyable{
public:
    explicit Thread(const ThreadFunc& func, const std::string& name = std::string());
    ~Thread();

    void start();
    int join();

private:
    //区分pthread_t和pid_t
    //https://blog.csdn.net/zxc120389574/article/details/105201003
    pthread_t tid_;
    pid_t pid_;          //用pid来区分不同的线程

    std::string name_;
    ThreadFunc func_;
    bool isStarted;
    bool isJoined;
    MutexLock mutex_;
    Condition cond_;
};

