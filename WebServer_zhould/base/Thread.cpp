//
// Created by zhoujc on 2020/6/6.
//
#include <assert.h>
#include <memory>
#include <unistd.h>
#include "Thread.h"
#include "CurrentThread.h"

struct threadData{
    threadData(Condition* cond, MutexLock* mutex, pid_t * pid, const ThreadFunc func)
        : cond_(cond),
          mutex_(mutex),
          pid_(pid),
          func_(func)
          {};

    Condition* cond_;
    MutexLock* mutex_;
    pid_t* pid_;
    ThreadFunc func_;
};

void* startFunc(void* obj){
    threadData* dataObj = (threadData*)obj;

    {
        MutexLockGuard mutexGuard(*(dataObj->mutex_));
        *(dataObj->pid_) = CurrentThread::tid();
        dataObj->cond_->notify();
    }

    dataObj->func_();

}

Thread::Thread(const ThreadFunc& func, const std::string& name)
    : func_(func),
      name_(name),
      tid_(0),
      pid_(0),
      isStarted_(false),
      isJoined(false),
      mutex_(),
      cond_(mutex_){
};

Thread::~Thread() {
    if(isStarted_ && !isJoined){
        pthread_detach(tid_);
    }
}

void Thread::start(){
    assert(!isStarted_);
    isStarted_ = true;
    threadData dataObj(&cond_, &mutex_, &pid_, func_);
    if(pthread_create(&tid_, nullptr, startFunc, &dataObj)){
        //创建线程出错
        isStarted_ = false;
    }else{
        MutexLockGuard mutexGuard(mutex_);
        while(!pid_) {
            cond_.wait();
        }
    }
}

int Thread::join() {
    assert(isStarted_);
    assert((!isJoined));

    isJoined = true;
    return pthread_join(tid_, nullptr);
}




