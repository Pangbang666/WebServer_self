//
// Created by zhoujc on 2020/6/15.
//
#pragma once

#include <syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace CurrentThread{
    extern __thread int t_cachedTid;

    void cacheTid();

    inline int tid(){
        if(__builtin_expect( t_cachedTid == 0, 0)){
            cacheTid();
        }
        return t_cachedTid;
    }
}
