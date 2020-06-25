//
// Created by zhoujc on 2020/6/11.
//
#pragma once

#include "../base/noncopyable.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <map>

class EventLoop;
class Channel;
class TimerNode;

enum ProcessState {
    STATE_PARSE_URI = 1,
    STATE_PARSE_HEADERS,
    STATE_RECV_BODY,
    STATE_ANALYSIS,
    STATE_FINISH
};

enum URIState {
    PARSE_URI_AGAIN = 1,
    PARSE_URI_ERROR,
    PARSE_URI_SUCCESS,
};

enum HeaderState {
    PARSE_HEADER_SUCCESS = 1,
    PARSE_HEADER_AGAIN,
    PARSE_HEADER_ERROR
};

enum AnalysisState { ANALYSIS_SUCCESS = 1, ANALYSIS_ERROR };

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

class MimeType {
private:
    static void init();
    static std::unordered_map<std::string, std::string> mime;
    MimeType();
    MimeType(const MimeType &m);

public:
    static std::string getMime(const std::string &suffix);

private:
    static pthread_once_t once_control;
};

class HttpData{
public:
    HttpData(EventLoop* loop, int fd);
    ~HttpData();

    void reset();

    void newEvent();
    void linkTimer(std::shared_ptr<TimerNode> timer);
    void seperateTimer();

public:
    void handleRead();
    void handleWrite();
    void handleConn();
    void handleClose();
    void handleError(int fd, int err_num, std::string short_msg);
    URIState parseURI();
    HeaderState parseHeaders();
    AnalysisState analysisRequest();

    std::shared_ptr<Channel> getChannel();
private:
    int fd_;
    EventLoop* loop_;
    std::shared_ptr<Channel> channel_;
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
    ParseState hState_;
    bool keepAlive_;
    std::map<std::string, std::string> headers_;
    std::weak_ptr<TimerNode> timer_;
};


