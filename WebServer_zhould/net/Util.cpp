//
// Created by zhoujc on 2020/6/18.
//
#include <unistd.h>
#include <errno.h>
#include "Util.h"

const int MAX_BUF = 4096;

ssize_t readn(int fd, void* buff, size_t n){
    ssize_t nleft = n;
    ssize_t nread = 0;
    ssize_t readSum = 0;
    char* ptr = (char*)buff;
    while(nleft > 0){
        if((nread = read(fd, ptr, nleft)) < 0){
            if(errno == EINTR)
                nread = 0;
            else if(errno == EAGAIN){
                return readSum;
            }else{
                return -1;
            }
        }else if(nread == 0)
            break;
        else{
            nleft -= nread;
            ptr += nread;
            readSum += nread;
            nread = 0;
        }
    }

    return readSum;
}

ssize_t readn(int fd, std::string& inBuffer, bool& zero){
    ssize_t nread = 0;
    ssize_t readSum = 0;
    while(true){
        char buf[MAX_BUF];
        if((nread = read(fd, buf, MAX_BUF)) < 0){
            if(errno == EINTR)
                continue;
            else if(errno == EAGAIN)   //表示当前缓冲区已无数据可读
                return readSum;
            else
                return -1;
        }else if(nread == 0) {
            zero = true;
            break;
        }

        readSum += nread;
        inBuffer += std::string(buf, buf + nread);
    }

    return readSum;
}

ssize_t writen(int fd, void *buff, size_t n) {
    size_t nleft = n;
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    char *ptr = (char *)buff;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0) {
                if (errno == EINTR) {
                    nwritten = 0;
                    continue;
                } else if (errno == EAGAIN) {
                    return writeSum;
                } else
                    return -1;
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return writeSum;
}

ssize_t writen(int fd, std::string &sbuff) {
    size_t nleft = sbuff.size();
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    const char *ptr = sbuff.c_str();
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0) {
                if (errno == EINTR) {
                    nwritten = 0;
                    continue;
                } else if (errno == EAGAIN){    //表示写缓冲队列已满
                    if( writeSum == sbuff.size())
                        sbuff.clear();
                    else
                        sbuff=sbuff.substr(writeSum);

                    return writeSum;
                }
                else
                    return -1;
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    if (writeSum == static_cast<int>(sbuff.size()))
        sbuff.clear();
    else
        sbuff = sbuff.substr(writeSum);
    return writeSum;
}