//
// Created by zhoujc on 2020/6/11.
//

#include <unistd.h>
#include "HttpData.h"

#define MAXSIZE 10240

HttpData::HttpData(int fd)
    : fd_(fd),
      readBuffer_(std::string()),
      writeBuffer_(std::string()){

}

HttpData::~HttpData() {
}

void HttpData::readFd() {
    readBuffer_.clear();
    char buffer[MAXSIZE];
    ssize_t len = read(fd_, (void*)buffer, MAXSIZE);
}