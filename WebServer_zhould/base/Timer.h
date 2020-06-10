//
// Created by zhoujc on 2020/6/9.
//
#include <memory>
#include "noncopyable.h"
#include <queue>
#include <sys/time.h>

class Channel;

struct TimeNode{
    TimeNode(std::shared_ptr<Channel> channel, size_t timeout){
        //根据timeout计算出超时时刻
        timeval curTime;
        gettimeofday(&curTime, nullptr);
        expiredTime_ = curTime.tv_sec * 1000 + curTime.tv_usec / 1000 + timeout;  //以毫秒计算
    }

    size_t expiredTime_;
    std::shared_ptr<Channel> channel_;
};

struct cmp{
    bool operator() (const TimeNode& node1, const TimeNode& node2){
        return node1.expiredTime_ < node2.expiredTime_;
    }
};

class TimerManager : noncopyable{
public:
    TimerManager() {};
    ~TimerManager() {};

    std::vector<std::shared_ptr<Channel>> tick(){
        timeval curTime;
        gettimeofday(&curTime, nullptr);
        size_t  time_now = curTime.tv_sec * 1000 + curTime.tv_usec / 1000 ;  //以毫秒计算

        std::vector<std::shared_ptr<Channel>> expiredChannels_;
        while(!q.empty()){
            TimeNode frontNode = q.top();
            if(frontNode.expiredTime_ < time_now){
                expiredChannels_.push_back(frontNode.channel_);
                q.pop();
            }else
                break;
        }

        return expiredChannels_;
    }

    void addTimer(std::shared_ptr<Channel> channel_, size_t timeout){
        TimeNode newTimer(channel_, timeout);
        q.push(newTimer);
    }

private:
    std::priority_queue<TimeNode, std::vector<TimeNode>, cmp> q;
};
