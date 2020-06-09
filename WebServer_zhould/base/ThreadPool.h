//
// Created by zhoujc on 2020/6/8.
//
#pragma once

#include <vector>
#include <memory>
#include "Thread.h"

class ThreadPool {
public:
    ThreadPool(int num);
    ~ThreadPool();

private:
    int threadNum_;
    std::vector<std::unique_ptr<Thread>> threads_;
};


