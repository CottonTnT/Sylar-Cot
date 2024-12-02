/**
 * @file thread.h
 * @brief 线程相关的封装
 * @author sylar.yin
 * @email 564628276@qq.com
 * @date 2019-05-31
 * @copyright Copyright (c) 2019年 sylar.yin All rights reserved (www.sylar.top)
 */
#ifndef __SYLAR_THREAD_H__
#define __SYLAR_THREAD_H__

#include "mutex.h"
#include <functional>
#include <unistd.h>
#include <memory>

namespace Cot {

/**
 * @brief 线程类
 */
class Thread : NonCopyable {
public:
    using ThreadFunc = std::function<void()>;

    Thread(ThreadFunc cb, const std::string& name);

    ~Thread();

    auto getThreadId() const
        -> pid_t
    {
        return m_id;
    }

    auto getName() const
        -> const std::string&
    {
        return m_name;
    }

    void join();

    static auto getLocalThread()
        -> Thread*;

    static auto getLocalThreadName()
        -> const std::string&;

    static void setLocalThreadName();

private:
    /**
     * @brief 线程执行函数
     */
    static void* run(void* arg);

private:
    pid_t m_id                = -1;
    pthread_t m_thread_handle = 0;
    ThreadFunc m_cb;
    std::string m_name;
    Semaphore m_sem;
};

} // namespace sylar

#endif