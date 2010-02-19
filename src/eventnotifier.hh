/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef SRC_GUARD_EVENTNOTIFIER_HH
#define SRC_GUARD_EVENTNOTIFIER_HH 1

#include <pthread.h>

namespace genesis
{
    class EventNotifier
    {
        private:
            pthread_cond_t * _cond;
            pthread_mutex_t * _cond_lock;
            pthread_mutex_t * _lock;

        public:
            EventNotifier() : _cond(new pthread_cond_t), _cond_lock(new pthread_mutex_t), _lock(new pthread_mutex_t) {}
            void lock();
            void unlock();
            void broadcast();
            void signal();
            void wait();
    };
}

#endif
