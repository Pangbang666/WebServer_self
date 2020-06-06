set(SRCS
    main.cpp base/MutexLock.h base/noncopyable.h base/Thread.h base/Condition.h base/Condition.cpp base/Thread.cpp)

add_executable(WebServer_zhould ${SRCS})

find_package(Threads REQUIRED)
target_link_libraries(WebServer_zhould Threads::Threads)