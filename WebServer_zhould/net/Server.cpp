//
// Created by zhoujc on 2020/6/11.
//

#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>

#define MAXLISTENNUN 1024
#define DEFAULT_TIMEOUT 2048

static int creatListenFd(int port){
    int listenFd;
    socklen_t len;
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = port;

    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenFd, (sockaddr*)(&serverAddr), sizeof(serverAddr));

    return listenFd;
}

Server::Server(EventLoop* loop, size_t threadNum, int port)
    : loop_(loop),
      threadNum_(threadNum),
      threadPool_(new EventLoopThreadPool(threadNum)),
      port_(port),
      listenFd_(creatListenFd(port)),
      acceptChannel_(new Channel(listenFd_)){

    __uint32_t events = EPOLLIN | EPOLLOUT | EPOLLET;
    acceptChannel_->setEvents(events);
    acceptChannel_->setReadCallback(std::bind(&Server::acceptFunc, this));
}

Server::~Server() {

}

void Server::start() {
    //启动线程池
    threadPool_->start();

    //监听端口
    listen(listenFd_, MAXLISTENNUN);
    loop_->addChannel(acceptChannel_);
}

void Server::acceptFunc() {
    sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    int connFd_;
    bzero(&clientAddr, sizeof((clientAddr)));

    connFd_ = accept(listenFd_, (sockaddr*)(&clientAddr), &len);
    assert(connFd_ != -1);

    EventLoop* loop = threadPool_->getNextLoop();
    std::shared_ptr<Channel> newChannel(new Channel(connFd_));
    loop->addChannel(newChannel, DEFAULT_TIMEOUT);
}
