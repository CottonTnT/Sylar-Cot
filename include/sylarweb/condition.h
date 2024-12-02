#ifndef CONDITION_H
#define CONDITION_H

#include <mutex>
#include <condition_variable>
#include <pthread.h>
#include <thread>
#include "sylarweb/noncopyable.h"
#include "sylarweb/mutex.h"

namespace Cot {

class Condition : public NonCopyOrMoveable {

public:
    using MutexType = Cot::Mutex;
    Condition()
    {
        CCHECK(pthread_cond_init(&m_cond, nullptr));
    }

    ~Condition()
    {
        MCHECK(pthread_cond_destroy(&m_cond));
    }

    void wait(MutexType& mutex)
    {
        pthread_cond_wait(&m_cond, mutex.getPthreadMutex());
    }

    template <typename Predicate>
    void wait(MutexType& mutex, Predicate pred)
    {
        while (pred)
        {
            wait(mutex);
        }
    }

    void notify()
    {
        CCHECK(pthread_cond_signal(&m_cond));
    }

    void notifyAll()
    {
        MCHECK(pthread_cond_broadcast(&m_cond));
    }

private:
    pthread_cond_t m_cond;
};

} //namespace Cot
#endif