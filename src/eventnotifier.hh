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

            // Lock EventNotifier object using a mutex
            void lock();

            // Unlock EventNotifier object
            void unlock();

            // Signal all threads waiting on the condition variable
            void broadcast();

            // Signal one thread waiting on the condition variable
            void signal();

            // Wait for signal
            void wait();

            // Returns a pointer to given Action object of the current event
            Action * getaction();

            // Sets an Action for the current event
            void setaction(Action * action);
    };
}

#endif
