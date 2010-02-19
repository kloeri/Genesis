/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <iostream>
#include <pthread.h>
#include "eventnotifier.hh"

namespace genesis
{
    EventNotifier::EventNotifier() :
        _cond(new pthread_cond_t),
        _cond_lock(new pthread_mutex_t),
        _lock(new pthread_mutex_t)
    {
        pthread_cond_init(_cond, 0);
        pthread_mutex_init(_cond_lock, 0);
        pthread_mutex_init(_lock, 0);
    }

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

    Action * EventNotifier::getaction()
    {
        return _action;
    }

    void EventNotifier::setaction(Action * action)
    {
        _action = action;
    }
}

