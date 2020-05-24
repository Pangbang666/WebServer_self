//
// Created by zhoujc on 2020/5/4.
//
#pragma once
#include <memory>
#include <string>
#include <unistd.h>
#include "Timer.h"
#include "Channel.h"
#include <unordered_map>
#include <map>

class EventLoop;

using namespace std;

enum ProcessState{
    STATE_PARSE_URI=1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};

enum URIState{
    PARSE_URI_AGAIN=1,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS
};

enum HeaderState{
    PARSE_HEADER_SUCCESS=1,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
};

enum AnalysisState{
    ANALYSIS_SUCCESS=1,
    ANALYSIS_ERROR
};

enum ParseState {
    H_START = 0,
    H_KEY,
    H_COLON,
    H_SPACES_AFTER_COLON,
    H_VALUE,
    H_CR,
    H_LF,
    H_END_CR,
    H_END_LF
};

enum ConnectionState { H_CONNECTED = 0, H_DISCONNECTING, H_DISCONNECTED };

enum HttpMethod { METHOD_POST = 1, METHOD_GET, METHOD_HEAD };

enum HttpVersion { HTTP_10 = 1, HTTP_11 };

class MimeType{
private:
    static void init();
    static std::unordered_map<std::string,std::string> mime;
    MimeType();
    MimeType(const MimeType& m);

public:
    static std::string getMime(const std::string &suffix);

private:
    //pthread_once详解
    //https://blog.csdn.net/rain_qingtian/article/details/11479737?ops_request_misc=&request_id=&biz_id=102&utm_term=pthread_once_t%E6%98%AF%E4%BB%80%E4%B9%88%E7%B1%BB%E5%9E%8B&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-0-11479737
    static pthread_once_t once_control;
};

//enable_shared_from_this使用总结
//https://blog.csdn.net/xjc200808/article/details/46506679?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522158979715819725211959627%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=158979715819725211959627&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v3~pc_rank_v3-2-46506679.first_rank_ecpm_v3_pc_rank_v3&utm_term=shared_from_this是干嘛用的
class HttpData : std::enable_shared_from_this<HttpData> {
public:
    HttpData(EventLoop *loop, int connfd);
    ~HttpData() { close(fd_);}
    void reset() ;
    void seperateTimer();
    void linkTimer(std::shared_ptr<TimerNode> timer) {timer_= timer;}

    std::shared_ptr<Channel> getChannel() { return channel_;}
    EventLoop* getLoop() { return loop_;}
    void handleClose();
    void newEvent();

private:
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
    int fd_;
    std::string inBuffer_;
    std::string outBuffer_;
    bool error_;
    ConnectionState connectionState_;

    HttpMethod method_;
    HttpVersion HTTPVersion_;
    std::string fileName_;
    std::string path_;
    int nowReadPos_;
    ProcessState state_;
    ParseState  hState_;
    bool keepAlive_;
    std::map<std::string,std::string> headers_;
    std::weak_ptr<TimerNode> timer_;

    void handleRead();
    void handleWrite();
    void handleConn();
    void handleError(int fd, int err_num, std::string short_msg);
    URIState parseURI();
    HeaderState parseHeaders();
    AnalysisState analysisState();
};


