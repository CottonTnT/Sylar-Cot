#include "sylarweb/thread.h"
#include "sylarweb/log.h"

namespace Cot {

static thread_local auto* t_thread     = static_cast<Thread*>(nullptr);
static thread_local auto t_thread_name = std::string("UNKNOW");

// static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

using ThreadFunc = std::function<void()>;

auto Thread::getLocalThread()
    -> Thread*
{
    return t_thread;
}

auto Thread::getLocalThreadName()
    -> const std::string&
{
    return t_thread_name;
}

Thread::Thread(ThreadFunc cb, const std::string& name)
    : m_cb(cb)
    , m_name(name)
{
    if (name.empty())
    {
        m_name = "UNKNOW";
    }
    auto ret_val = pthread_create(&m_thread_handle, nullptr, &Thread::run, this);
    if (ret_val)
    {
        // SYLAR_LOG_ERROR(g_logger) << "pthread_create thread fail, rt=" << rt
        //                           << " name=" << name;
        throw std::logic_error("pthread_create error");
    }
    m_sem.wait();
}

Thread::~Thread()
{
    if (m_thread_handle != 0)
    {
        pthread_detach(m_thread_handle);
    }
}

void Thread::join()
{
    if (m_thread_handle != 0)
    {
        auto ret_val = pthread_join(m_thread_handle, nullptr);
        if (ret_val)
        {
            //  SYLAR_LOG_ERROR(g_logger) << "pthread_join thread fail, rt=" << rt
            //                           << " name=" << m_name;
            throw std::logic_error("pthread_join error");
        }
        m_thread_handle = 0;
    }
}

auto Thread::run(void* arg)
    -> void*
{
    auto* thr     = static_cast<Thread*>(arg);
    t_thread      = thr;
    t_thread_name = thr->getName();
    thr->m_id     = Sylar::getThreadId();
    pthread_setname_np(pthread_self(), thr->m_name.substr(0, 15).c_str());

    auto cb = ThreadFunc();
    cb.swap(thr->m_cb);

    thr->m_sem.notify();

    cb();
    return 0;
}

} //namespacem