//
// Created by zhoujc on 2020/6/18.
//
#include <unistd.h>
#include <errno.h>
#include "Util.h"

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