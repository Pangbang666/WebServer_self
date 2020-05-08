//
// Created by zhoujc on 2020/5/1.
//
#include "Server.h"
#include "stdio.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include "Util.h"

Server::Server(EventLoop *loop, int threadNum, int port)
    : loop_(loop),
      threadNum_(threadNum),
      port_(port),
      eventLoopThreadPool_(new EventLoopThreadPool(loop_,threadNum_)),
      acceptChannel_(new Channel(loop_)),
      started_(false),
      listenFd_(socket_bind_listen(port_)){
    acceptChannel_->setFd(listenFd_);
    handle_for_sigpipe();
    if (setSocketNonBlocking(listenFd_) < 0) {
        perror("set socket non block failed");
        abort();
    }
}

void Server::start() {
    assert(!started_);
    eventLoopThreadPool_->start();
    acceptChannel_->setEvents(EPOLLIN||EPOLLET);
    acceptChannel_->setReadHandler(bind(&Server::handleNewConn,this));
    acceptChannel_->setConnHandler(bind(&Server::handleThisConn,this));
    loop_->addToPoller(acceptChannel_,0);
    started_=true;
}

void Server::handleNewConn() {
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(struct sockaddr_in));
    socklen_t client_addr_len=sizeof(client_addr);
    int accept_fd=0;
    while((accept_fd=accept(listenFd_,(struct sockaddr*)(&client_addr),&client_addr_len)>0){
        EventLoop* loop=eventLoopThreadPool_->getNextLoop();
        LOG << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":"
            << ntohs(client_addr.sin_port);

        //限制服务器的最大并发连接数
        if(accept_fd>=MAXFDS){
            close(accept_fd);
            continue;
        }

        //设为非阻塞模式
        if(setSocketNonBlocking(accept_fd)<0){
            LOG::<<"Set no blocked failed!";
            return;
        }

        setSocketNodeLay(accept_fd);

        std::shared_ptr<HttpData> req_info(new HttpData(loop,accept_fd));
        req_info->getChannel()->setHolder(req_info);
        loop->queueInLoop(std::bind(&HttpData::newEvent,req_info));
    }
    acceptChannel_->setEvents(EPOLLIN||EPOLLET);
}