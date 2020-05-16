cmake_minimum_required(VERSION 2.8)
project(WebServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(WebServer_self WebServer/main.cpp WebServer/Server.cpp WebServer/Server.h WebServer/EventLoop.cpp WebServer/EventLoop.h WebServer/Epoll.cpp WebServer/Epoll.h WebServer/base/noncopyable.h WebServer/base/MutexLock.cpp WebServer/base/MutexLock.h WebServer/Channel.cpp WebServer/Channel.h WebServer/HttpData.cpp WebServer/HttpData.h WebServer/Timer.cpp WebServer/Timer.h WebServer/base/CurrentThread.h WebServer/Util.cpp WebServer/Util.h WebServer/base/Condition.h WebServer/base/Thread.cpp WebServer/base/Thread.h WebServer/base/CountDownLatch.cpp WebServer/base/CountDownLatch.h WebServer/EventLoopThread.cpp WebServer/EventLoopThread.h WebServer/EventLoopThreadPool.cpp WebServer/EventLoopThreadPool.h WebServer/base/FileUtil.cpp WebServer/base/FileUtil.h WebServer/base/LogFile.cpp WebServer/base/LogFile.h WebServer/base/AsyncLogging.cpp WebServer/base/AsyncLogging.h WebServer/base/LogStream.cpp WebServer/base/LogStream.h WebServer/base/Logging.cpp WebServer/base/Logging.h)

find_package(Threads REQUIRED)
target_link_libraries(WebServer_self Threads::Threads)