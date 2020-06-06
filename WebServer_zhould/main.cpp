//
// Created by zhoujc on 2020/5/30.
//
#include <iostream>
#include "base/Thread.h"

MutexLock mutex_;
Condition cond_(mutex_);
bool index = false;

void func(){
    std::cout << "HELLO" <<std::endl;

    MutexLockGuard guard(mutex_);
    index = true;
    cond_.notifyAll();
}

int main(){

    Thread t(std::bind(&func), "my thread");
    t.start();

    {
        MutexLockGuard guard(mutex_);
        while(!index){
            cond_.wait();
        }
    }

    int a =1;

    return 0;
}
