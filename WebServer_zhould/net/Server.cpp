//
// Created by zhoujc on 2020/6/11.
//

#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#define MAXLISTENNUN 1024
#define DEFAULT_TIMEOUT 2048

static int creatListenFd(int port){
    //端口取正确的范围
    if( port < 0 || port >65535)
        return -1;

    //创建socket（IPv4 + TCP），并返回监听描述符
    int listenFd = 0;
    if((listenFd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;

    //消除bind时“Address is already in use”错误
    int optval = 1;
    if(setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
        close(listenFd);
        return -1;
    }

    //设置服务器Ip和Port，并绑定监听描述符
    struct sockaddr_in serverAddr;
    bzero((char*)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons((unsigned short)port);
    if(bind(listenFd, (struct sockaddr*)(&serverAddr), sizeof(serverAddr)) == -1){
        close(listenFd);
        return -1;
    }

    //开始监听，并设置最大监听队列长度
    if(listen(listenFd, MAXLISTENNUN) == -1){
        close(listenFd);
        return -1;
    }

    //无效监听描述符
    if(listenFd == -1){
        close(listenFd);
        return -1;
    }

    return listenFd;
}

static void hand_for_sigpipe(){
    struct sigaction sa;
    bzero(&sa, sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, nullptr))
        return;
}

static int setSocketNonBlocking(int fd){
    int flag = fcntl(fd, F_GETFL, 0);
    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1){
        return -1;
    }

    return 0;
}

static void setSocketNoDelay(int fd){
    int enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void*)&enable, sizeof(enable));
}

Server::Server(EventLoop* loop, size_t threadNum, int port)
    : loop_(loop),
      threadNum_(threadNum),
      threadPool_(new EventLoopThreadPool(threadNum_)),
      port_(port),
      listenFd_(creatListenFd(port)),
      acceptChannel_(new Channel(loop_, listenFd_)){
    acceptChannel_->setFd(listenFd_);
    hand_for_sigpipe();
    assert( setSocketNonBlocking(listenFd_) >= 0);
}

Server::~Server() {

}

void Server::start() {
    //启动线程池
    threadPool_->start();

    acceptChannel_->setEvents(EPOLLIN | EPOLLET);
    acceptChannel_->setReadCallback(std::bind(&Server::acceptFunc, this));
    loop_->addChannel(acceptChannel_);
}

void Server::acceptFunc() {
    sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    int acceptFd_;
    bzero(&clientAddr, sizeof((clientAddr)));
    while((acceptFd_ = accept(listenFd_, (sockaddr*)(&clientAddr), &len)) > 0){

        //限制服务器的最大并发连接数
        if(acceptFd_ >= MAXLISTENNUN){
            close(acceptFd_);
            continue;
        }

        //设置为非阻塞模式
        if( setSocketNonBlocking(acceptFd_) < 0)
            return;

        setSocketNoDelay(acceptFd_);

        std::shared_ptr<HttpData> httpData_(new HttpData(loop_, acceptFd_));
        loop_->queueInLoop(std::bind(&HttpData::newEvent, httpData_));
    }
    acceptChannel_->setEvents(EPOLLIN | EPOLLET);
}
