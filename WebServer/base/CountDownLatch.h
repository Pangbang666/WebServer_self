//
// Created by zhoujc on 2020/5/7.
//
#pragma once
#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

//用以确保Thread传入的func启动后，外层的start才返回
class CountDownLatch : noncopyable{
public:
    explicit CountDownLatch(int count);
    void wait();
    void countDown();

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};

