//
// Created by zhoujc on 2020/5/4.
//
#pragma once

class noncopyable
{
protected:
    noncopyable() {};
    ~noncopyable() {};

private:
    noncopyable(const noncopyable&);
    noncopyable& operator=(const noncopyable&);
};
