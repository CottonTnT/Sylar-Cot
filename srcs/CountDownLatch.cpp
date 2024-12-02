#include "sylarweb/mutex.h"
#include "sylarweb/countdownlatch.h"

namespace Cot {

CountDownLatch::CountDownLatch(int count)
    : m_count(count)
{
}

void CountDownLatch::wait()
{
    m_mutex.lock();
    //note:感觉这样写，降低了可读性
    // m_cond.wait(m_mutex, [this]() {
    //     return m_count > 0;
    // });

    while (m_count > 0)
    {
        m_cond.wait(m_mutex);
    }
}

void CountDownLatch::countDown()
{
    auto lk_guard = LockGuard<Mutex>(m_mutex);
    m_count--;
    if (m_count == 0)
    {
        m_cond.notifyAll();
    }
}

auto CountDownLatch::getCount() const
    -> int
{
    auto lk_guard = LockGuard<Mutex>(m_mutex);
    return m_count;
}

} //namespace Cot