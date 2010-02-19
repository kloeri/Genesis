/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <pthread.h>
#include "eventnotifier.hh"

namespace genesis
{
    void EventNotifier::lock()
    {
        pthread_mutex_lock(_lock);
    }

    void EventNotifier::unlock()
    {
        pthread_mutex_unlock(_lock);
    }

    void EventNotifier::broadcast()
    {
        pthread_cond_broadcast(_cond);
    }

    void EventNotifier::signal()
    {
        pthread_cond_signal(_cond);
    }

    void EventNotifier::wait()
    {
        pthread_cond_wait(_cond, _cond_lock);
    }
}

