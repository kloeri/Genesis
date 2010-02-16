/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef SRC_GUARD_EVENTS_EVENT_HH
#define SRC_GUARD_EVENTS_EVENT_HH 1

class EventManager
{
    public:
        virtual void *GetEvent() = 0;
        virtual ~EventManager() {}
};

#endif
