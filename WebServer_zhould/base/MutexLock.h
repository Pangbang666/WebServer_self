//
// Created by zhoujc on 2020/6/4.
//
#pragma once
#include "noncopyable.h"
#include <pthread.h>

class MutexLock{
public:
    MutexLock() { pthread_mutex_init(&mutex, nullptr);}
    ~MutexLock(){
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }

    void lock() { pthread_mutex_lock(&mutex);}
    void unlock() { pthread_mutex_unlock(&mutex);}
    pthread_mutex_t* get() {return &mutex;}

private:
    pthread_mutex_t mutex;
};

class MutexLockGuard{
public:
    explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex) { mutex_.lock();}
    ~MutexLockGuard() { mutex_.unlock();}

private:
    MutexLock& mutex_;
};
