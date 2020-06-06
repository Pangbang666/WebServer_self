//
// Created by zhoujc on 2020/6/6.
//
#pragma once

#include <pthread.h>
#include "noncopyable.h"
#include <memory>
#include "MutexLock.h"

class Condition : noncopyable{
public:
    Condition(MutexLock& mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();

private:
    pthread_cond_t cond_;
    MutexLock& mutex_;
};
