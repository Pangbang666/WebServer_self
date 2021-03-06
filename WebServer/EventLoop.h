//
// Created by zhoujc on 2020/5/3.
//
#pragma once

#include <functional>
#include <memory>
#include <sys/types.h>
#include "base/MutexLock.h"
#include <assert.h>
#include "Channel.h"
#include "Epoll.h"
#include <vector>
#include "Util.h"
#include "base/CurrentThread.h"

using namespace std;

class EventLoop {
public:
    typedef std::function<void()> Functor;
    typedef std::shared_ptr<Channel> SP_Channel;
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();
    void runInLoop(Functor&& cb);
    void queueInLoop(Functor&& cb);
    bool isInLoopThread() { return threadId_==CurrentThread::tid();}
    void assertInLoopThread() { assert(isInLoopThread()); }
    void shutdown(shared_ptr<Channel> channel) { shutDownWR(channel->getFd());}

    void removeFromPoller(shared_ptr<Channel> channel){
        poller_->epoll_del(channel);
    }

    void updatePoller(shared_ptr<Channel> channel, int timeout=0){
        poller_->epoll_mod(channel,timeout);
    }

    void addToPoller(shared_ptr<Channel> channel, int timeout=0){
        poller_->epoll_add(channel,timeout);
    }


private:
    bool looping_;
    int wakeupFd_;
    bool quit_;
    bool eventHandling_;
    mutable MutexLock mutex_;
    shared_ptr<Epoll> poller_;
    std::vector<Functor> pendingFunctors_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    shared_ptr<Channel> pwakeupChannel_;

    void wakeup();
    void handleRead();
    void doPendingFunctors();
    void handleConn();
};


