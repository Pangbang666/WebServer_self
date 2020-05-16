//
// Created by zhoujc on 2020/5/16.
//
#pragma once

#include <memory>
#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"


class LogFile {
public:
    //append函数被调用flushEveryN_次，flush函数调用一次；会🕸往文件写，只不过，文件也是带有缓冲的
    LogFile(const std::string& basename, int flushEveryN = 1024);
    ~LogFile();

    void append(const char* logline, int len);
    void flush();
    bool rollFile();

private:
    void append_unlocked(const char* logline, int len);

    const std::string basename_;
    const int flushEveryN_;

    int count_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;
};


