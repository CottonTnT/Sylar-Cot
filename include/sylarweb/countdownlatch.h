#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include "sylarweb/mutex.h"
#include "sylarweb/condition.h"

namespace Cot {

class CountDownLatch : NonCopyOrMoveable {

public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable Mutex m_mutex;
    Condition m_cond;
    int m_count;
};

} //namespace Cot

#endif