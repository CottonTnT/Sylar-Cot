#ifndef __SYLAR_MUTEX_GUARD__
#define __SYLAR_MUTEX_GUARD__

#include "noncopyable.h"
#include <pthread.h>
#include <semaphore.h>
#include <cstdint>
#include <mutex>

namespace Cot {

class Semaphore : NonCopyable {
public:
    /**
     * @brief 构造函数
     * @param[in] count 信号量值的大小
     */
    Semaphore(uint32_t count = 0);

    /**
     * @brief 析构函数
     */
    ~Semaphore();

    /**
     * @brief 获取信号量
     */
    void wait();

    /**
     * @brief 释放信号量
     */
    void notify();

private:
    sem_t m_semaphore;
};

/**
 * @brief 普通锁
 */
class Mutex : NonCopyOrMoveable {
public:
    /// 局部锁
    /**
     * @brief 构造函数
     */
    Mutex()
    {
        pthread_mutex_init(&m_mutex, nullptr);
    }

    /**
     * @brief 析构函数
     */
    ~Mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    /**
     * @brief 加锁
     */
    void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    /**
     * @brief 解锁
     */
    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

private:
    /// mutex
    pthread_mutex_t m_mutex;
};

/**
 * @brief 自旋锁
 */
class SpinMutex : NonCopyOrMoveable {
public:
    /**
     * @brief 构造函数
     */
    SpinMutex()
    {
        pthread_spin_init(&m_mutex, 0);
    }

    /**
     * @brief 析构函数
     */
    ~SpinMutex()
    {
        pthread_spin_destroy(&m_mutex);
    }

    /**
     * @brief 上锁
     */
    void lock()
    {
        pthread_spin_lock(&m_mutex);
    }

    /**
     * @brief 解锁
     */
    void unlock()
    {
        pthread_spin_unlock(&m_mutex);
    }

private:
    /// 自旋锁
    pthread_spinlock_t m_mutex;
};

} //namespace Sylar

#endif