//
// Created by zhoujc on 2020/6/8.
//

#include "EventLoop.h"
#include <assert.h>
#include <sys/eventfd.h>
#include "Util.h"

//eventfd函数
//https://blog.csdn.net/JiangXiaoPeng001/article/details/90770118?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522159204453419195162531142%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=159204453419195162531142&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-2-90770118.first_rank_ecpm_v1_pc_rank_v3&utm_term=eventfd%E5%87%BD%E6%95%B0
static int creatEventFd(){
    int eventFd_ = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    assert(eventFd_ >= 0);

    return eventFd_;
}

EventLoop::EventLoop()
    : pid_(CurrentThread::tid()),
      wakeupFd_(creatEventFd()),
      poller_(),
      started_(false),
      timerManager_(),
      eventhandling_(false),
      wakeupChannel_(new Channel(this, wakeupFd_)),
      mutex_(),
      callingPendingFunctors_(false){
    wakeupChannel_->setFd(wakeupFd_);
    wakeupChannel_->setEvents(EPOLLIN | EPOLLET);

    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    addChannel(wakeupChannel_, 0);
}

EventLoop::~EventLoop() {

}

void EventLoop::addChannel(std::shared_ptr<Channel> channel_, size_t timeout) {
    poller_.addEvent(channel_);

    if(timeout > 0){
        timerManager_.addTimer(channel_, timeout);
    }
}

void EventLoop::modChannel(std::shared_ptr<Channel> channel_, size_t timeout) {
    poller_.modEvent(channel_);

    if(timeout > 0){
        timerManager_.addTimer(channel_,timeout);
    }
}

void EventLoop::delChannel(std::shared_ptr<Channel> channel_, size_t timeout) {
    poller_.delEvent(channel_);

    if(timeout > 0){
        timerManager_.addTimer(channel_, timeout);
    }
}

void EventLoop::loop() {
    assert(isInThread());
    assert(!started_);

    started_ = true;
    while(true){
        std::vector<std::shared_ptr<Channel>> eventChannels_ = poller_.poll();
        eventhandling_ = true;
        for(size_t i = 0; i < eventChannels_.size(); i++){
            eventChannels_[i]->handleEvent();
        }
        eventhandling_ = false;

        doPendingFunctors();

        //超时处理
        std::vector<std::shared_ptr<Channel>> expiredChannels = timerManager_.tick();
    }
}

void EventLoop::queueInLoop(EventLoop::Functor &&cb) {
    {
        MutexLockGuard guard(mutex_);
        pendingFunctors_.push_back(cb);
    }

    if(!isInThread() || callingPendingFunctors_)
        wakeup();
}

void EventLoop::runInLoop(EventLoop::Functor &&cb) {
    if(isInThread())
        cb();
    else
        queueInLoop(std::move(cb));
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        MutexLockGuard guard(mutex_);
        std::swap(pendingFunctors_, functors);
    }

    for(int i = 0; i < functors.size(); i++)
        functors[i]();

    callingPendingFunctors_ = false;
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, (char*)&one, sizeof(one));
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = readn(wakeupFd_, &one, sizeof(one));

    wakeupChannel_->setEvents(EPOLLIN | EPOLLET);
}