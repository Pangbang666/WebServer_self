//
// Created by zhoujc on 2020/5/30.
//
#include <iostream>

#include "net/Server.h"
#include "net/EventLoop.h"

int main(){

    EventLoop loop;
    Server myServer(&loop, 1, 7778);
    myServer.start();
    loop.loop();

    return 0;
}
