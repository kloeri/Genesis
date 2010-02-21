/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef SRC_GUARD_EVENTS_EVENT_HH
#define SRC_GUARD_EVENTS_EVENT_HH 1

#include <pthread.h>

class EventManager
{
    pthread_cond_t main;
    pthread_cond_t handler;

    public:
        virtual void *GetEvent() = 0;
        virtual int get_fd() = 0;
        virtual ~EventManager() {}
};

#endif
