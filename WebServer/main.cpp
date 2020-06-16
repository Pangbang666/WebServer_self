//
// Created by zhoujc on 2020/5/1.
//
#include <iostream>
#include "Server.h"

int main()
{
    int threadNum=1;
    int port=7777;
    EventLoop mainLoop;
    Server myHTTPServer(&mainLoop, threadNum, port);
    myHTTPServer.start();
    mainLoop.loop();

    return 0;
}

