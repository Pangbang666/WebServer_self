cmake_minimum_required(VERSION 3.15)
project(WebServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(WebServer_self WebServer/main.cpp WebServer/Server.cpp WebServer/Server.h WebServer/EventLoop.cpp WebServer/EventLoop.h WebServer/Epoll.cpp WebServer/Epoll.h WebServer/base/noncopyable.h WebServer/base/MutexLock.cpp WebServer/base/MutexLock.h WebServer/Channel.cpp WebServer/Channel.h WebServer/HttpData.cpp WebServer/HttpData.h WebServer/Timer.cpp WebServer/Timer.h WebServer/base/CurrentThread.h WebServer/Util.cpp WebServer/Util.h WebServer/base/Condition.h WebServer/base/Thread.cpp WebServer/base/Thread.h WebServer/base/CountDownLatch.cpp WebServer/base/CountDownLatch.h)