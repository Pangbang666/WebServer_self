//
// Created by zhoujc on 2020/6/6.
//
#include "Condition.h"

Condition::Condition(MutexLock& mutex)
    : mutex_(mutex){
    pthread_cond_init(&cond_, nullptr);
}

Condition::~Condition() {
    pthread_cond_destroy(&cond_);
}

void Condition::wait() {
    pthread_cond_wait(&cond_, mutex_.get());
}

void Condition::notify() {
    pthread_cond_signal(&cond_);
}

void Condition::notifyAll() {
    pthread_cond_broadcast(&cond_);
}
