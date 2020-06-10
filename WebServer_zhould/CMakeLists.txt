set(SRCS
    main.cpp base/MutexLock.h base/noncopyable.h base/Thread.h base/Condition.h base/Condition.cpp base/Thread.cpp base/ThreadPool.cpp base/ThreadPool.h net/EventLoop.cpp net/EventLoop.h net/Channel.cpp net/Channel.h net/Poller.cpp net/Poller.h base/Timer.h net/EventLoopThread.cpp net/EventLoopThread.h net/EventLoopThreadPool.cpp net/EventLoopThreadPool.h)

add_executable(WebServer_zhould ${SRCS})

find_package(Threads REQUIRED)
target_link_libraries(WebServer_zhould Threads::Threads)