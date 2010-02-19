/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef SRC_GUARD_EVENTNOTIFIER_HH
#define SRC_GUARD_EVENTNOTIFIER_HH 1

#include <pthread.h>
#include <actions/action.hh>

namespace genesis
{
    class EventNotifier
    {
        private:
            pthread_cond_t * _cond;
            pthread_mutex_t * _cond_lock;
            pthread_mutex_t * _lock;
            Action * _action;

        public:
            EventNotifier();
            void lock();
            void unlock();
            void broadcast();
            void signal();
            void wait();
            Action * getaction();
            void setaction(Action * action);
    };
}

#endif
