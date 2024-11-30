#include <pthread.h>
#include <unistd.h>
class MutexBase {
public:
    using MutexType   = pthread_mutex_t;
    MutexType m_mutex = PTHREAD_MUTEX_INITIALIZER;
    MutexBase();
    ~MutexBase();

private:
};