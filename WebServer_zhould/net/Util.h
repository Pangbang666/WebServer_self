//
// Created by zhoujc on 2020/6/18.
//
#pragma once

#include <stdio.h>
#include <string>

ssize_t readn(int fd, void* buff, size_t n);
ssize_t readn(int fd, std::string& inBuffer, bool& zero);
ssize_t writen(int fd, void *buff, size_t n);
ssize_t writen(int fd, std::string &sbuff);
