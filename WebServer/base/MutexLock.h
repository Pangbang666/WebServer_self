//
// Created by zhoujc on 2020/5/4.
//
#pragma once
#include "noncopyable.h"
#include <pthread.h>

class MutexLock: noncopyable {
public:
    MutexLock() {pthread_mutex_init(&mutex, nullptr);}
    ~MutexLock()
    {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }

    void lock() {pthread_mutex_lock(&mutex);}
    void unlock() {pthread_mutex_unlock(&mutex);}

    pthread_mutex_t* get() {return &mutex;}

private:
    pthread_mutex_t mutex;
};

class MutexLockGuard:noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& _mutex):mutex(_mutex) {mutex.lock();}
    ~MutexLockGuard() {mutex.unlock();}
private:
    MutexLock &mutex;
};



