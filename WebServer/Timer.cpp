//
// Created by zhoujc on 2020/5/4.
//
#include "Timer.h"
#include <sys/time.h>
#include "HttpData.h"


TimerNode::TimerNode(std::shared_ptr<HttpData> requestData, int timeout)
    : deleted_(false), SPHttpData_(requestData)
{
    struct timeval now;
    gettimeofday(&now, nullptr);

    //计算超时的时刻（以毫秒计）
    expiredTime_=(((now.tv_sec%10000)*1000)+(now.tv_usec/1000))+timeout;
}

TimerNode::~TimerNode()
{
    if(SPHttpData_)
    {
        SPHttpData_->handleClose();
    }
}

TimerNode::TimerNode(TimerNode& tn)
    : SPHttpData_(tn.SPHttpData_), expiredTime_(0) {};

void TimerNode::update(int timeout)
{
    struct timeval now;
    gettimeofday(&now, nullptr);

    //计算超时的时刻（以毫秒计）
    expiredTime_=(((now.tv_sec%10000)*1000)+(now.tv_usec/1000))+timeout;
}

bool TimerNode::isValid()
{
    //timeval使用和理解
    //https://blog.csdn.net/engineer_james/article/details/84345380?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase
    struct timeval now;
    gettimeofday(&now, nullptr);

    //计算超时的时刻（以毫秒计）
    size_t temp =(((now.tv_sec%10000)*1000)+(now.tv_usec/1000));
    if(temp < expiredTime_)
        return true;
    else
    {
        setDeleted();
        return false;
    }
}

void TimerNode::clearReq()
{
    SPHttpData_.reset();
    setDeleted();
}

void TimerManager::addTimer(std::shared_ptr<HttpData> SPHttpData, int timeout)
{
    SPTimerNode new_node(new TimerNode(SPHttpData,timeout));
    timerNodeQueue.push(new_node);
    SPHttpData->linkTimer(new_node);
}

void TimerManager::handleExpiredEvent()
{
    while(!timerNodeQueue.empty())
    {
        SPTimerNode pTimer_now=timerNodeQueue.top();
        if(pTimer_now->isDeleted())
            timerNodeQueue.pop();
        else if(!pTimer_now->isValid())
            timerNodeQueue.pop();
        else
            break;
    }
}