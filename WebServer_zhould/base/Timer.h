//
// Created by zhoujc on 2020/6/9.
//
#include <memory>
#include "noncopyable.h"
#include <queue>
#include <sys/time.h>

class Channel;

struct TimerNode{
    TimerNode(std::shared_ptr<HttpData> httpData, size_t timeout)
        : httpData_(httpData),
          deleted_(false){
        //根据timeout计算出超时时刻
        timeval curTime;
        gettimeofday(&curTime, nullptr);
        expiredTime_ = (curTime.tv_sec % 10000 )* 1000 + curTime.tv_usec / 1000 + timeout;  //以毫秒计算
    }

    ~TimerNode(){
        if(httpData_)
            httpData_->handleClose();
    }

    void setDeleted() {deleted_ = true;}
    bool isDeleted() {return deleted_;}

    bool isValid(){
        timeval curTime;
        gettimeofday(&curTime, nullptr);
        size_t  time_now = (curTime.tv_sec % 10000) * 1000 + curTime.tv_usec / 1000 ;  //以毫秒计算

        if(time_now > expiredTime_){
            setDeleted();
            return false;
        }else
            return true;
    }

    void clearReq(){
        setDeleted();
        httpData_.reset();
    }

    bool deleted_;
    size_t expiredTime_;
    std::shared_ptr<HttpData> httpData_;
};

struct cmp{
    bool operator() (const std::shared_ptr<TimerNode>& node1, const std::shared_ptr<TimerNode>& node2) const {
        return node1->expiredTime_ < node2->expiredTime_;
    }
};

class TimerManager : noncopyable{
public:
    TimerManager() {};
    ~TimerManager() {};

    void handleExpired(){
        while(!timerNodeQueue_.empty()) {
            SPTimerNode frontNode = timerNodeQueue_.top();
            if(frontNode->isDeleted())
                timerNodeQueue_.pop();
            else if (!frontNode->isValid()) {
                timerNodeQueue_.pop();
            } else
                break;
        }
    }

    void addTimer(std::shared_ptr<HttpData> httpData_, size_t timeout){
        SPTimerNode newTimer(new TimerNode(httpData_, timeout));
        timerNodeQueue_.push(newTimer);
        httpData_->linkTimer(newTimer);
    }

private:
    typedef std::shared_ptr<TimerNode> SPTimerNode;
    std::priority_queue<SPTimerNode, std::deque<SPTimerNode>, cmp> timerNodeQueue_;
};
