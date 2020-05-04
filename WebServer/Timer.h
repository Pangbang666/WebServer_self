//
// Created by zhoujc on 2020/5/4.
//
#pragma once
#include "HttpData.h"
#include <memory>
#include <queue>
#include <deque>

using  namespace std;

class TimerNode {
public:
    TimerNode(std::shared_ptr<HttpData> requestData, int timeout);
    ~TimerNode();
    TimerNode(TimerNode& tn);
    void update(int timeout);
    bool isValid();
    void clearReq();
    void setDeleted() { deleted_==true; }
    bool isDeleted() { return deleted_; }
    size_t getExpTime() { return expiredTime_;}
private:
    bool deleted_;
    size_t expiredTime_;
    std::shared_ptr<HttpData> SPHttpData_;
};

struct TimerCmp
{
    bool operator()(std::shared_ptr<TimerNode>& first, std::shared_ptr<TimerNode>& second) const
    {
        return first->getExpTime()>second->getExpTime();
    }
};

class TimerManager
{
public:
    TimerManager() {};
    ~TimerManager() {};

    void addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout);
    void handleExpiredEvent();

private:
    typedef std::shared_ptr<TimerNode> SPTimerNode;
    std::priority_queue<SPTimerNode,deque<SPTimerNode>,TimerCmp> timerNodeQueue;
};
