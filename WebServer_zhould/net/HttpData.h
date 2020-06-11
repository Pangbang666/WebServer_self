//
// Created by zhoujc on 2020/6/11.
//
#pragma once

#include "../base/noncopyable.h"
#include <string>

class HttpData : noncopyable {
public:
    HttpData(int fd);
    ~HttpData();

public:
    void readFd();
private:
    int fd_;
    std::string readBuffer_;
    std::string writeBuffer_;
};


