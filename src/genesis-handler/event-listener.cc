/* vim: set sw=4 sts=4 et foldmethod=syntax : */
/*
 * Copyright (c) 2010 Bryan Østergaard
 *
 * This file is part of the Genesis initsystem. Genesis is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * Genesis is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <memory>

#include "genesis-handler/event-listener.hh"
#include "util/log.hh"

#define EVENT_LISTENER          ("Event-Listener")

using namespace genesis;
using namespace genesis::events;
using namespace genesis::logging;

EventListener::EventListener(void)
    : _terminate(false)
{
}

EventListener::~EventListener(void)
{
    std::map<int, EventManager *>::iterator manager;
    std::map<int, EventSource *>::iterator source;

    for (auto manager : _managers)
    {
        delete manager.second;
    }

    for (auto source : _sources)
    {
        delete source.second;
    }
}

void
EventListener::add_source(EventSource *source)
{
    int fd(source->fd());
    _sources[fd] = source;
}

void
EventListener::add_eventsource(EventManager *manager)
{
    int fd(manager->get_fd());
    _managers[fd] = manager;

    // Grab all 'coldplug' events
    std::list<std::string> coldplug_events = manager->get_events();
    _events.splice(_events.end(), coldplug_events);
}

void
EventListener::add_event(const std::string & event)
{
    _events.push_back(event);
}

void
EventListener::listen()
{
    int maxfd = 0;
    fd_set readfds;
    FD_ZERO(&readfds);
    std::map<int, EventManager *>::const_iterator manager;
    std::map<int, EventSource *>::const_iterator source;

    for (auto manager : _managers)
    {
        if (manager.first >= 0)
        {
            FD_SET(manager.first, &readfds);
            maxfd = std::max(manager.first, maxfd);
        }
    }

    for (auto source : _sources)
    {
        if (source.first >= 0)
        {
            FD_SET(source.first, &readfds);
            maxfd = std::max(source.first, maxfd);
        }
    }

    ::select(maxfd + 1, &readfds, NULL, NULL, NULL);

    for (auto manager : _managers)
    {
        if (FD_ISSET(manager.first, &readfds))
        {
            std::unique_ptr<Action> action(manager.second->GetEvent());

            if (action.get())
            {
                Log::get_instance().log(INFO, EVENT_LISTENER, "received action: " + action->Identity());
                action->Execute();
                Log::get_instance().log(DEBUG, EVENT_LISTENER, "action result: " + action->GetResult());

                _events.push_back(action->Identity());
            }
        }
    }

    for (auto source : _sources)
    {
        if (FD_ISSET(source.first, &readfds))
        {
            std::unique_ptr<Action> action(source.second->process_event());

            if (action.get())
            {
                Log::get_instance().log(INFO, EVENT_LISTENER, "received action: " + action->Identity());
                action->Execute();
                Log::get_instance().log(DEBUG, EVENT_LISTENER, "action result: " + action->GetResult());

                _events.push_back(action->Identity());
            }
        }
    }
}

void
EventListener::process_eventqueue(void)
{
    std::list<std::string>::const_iterator event;
    std::map<int, EventManager *>::const_iterator manager;

    for (auto event : _events)
    {
        for (auto manager : _managers)
        {
            std::unique_ptr<Action> action(manager.second->new_event(event));

            if (action.get())
            {
                Log::get_instance().log(INFO, EVENT_LISTENER, "received action: " + action->Identity());
                action->Execute();
                Log::get_instance().log(DEBUG, EVENT_LISTENER, "action result: " + action->GetResult());

                _events.push_back(action->Identity());
            }
        }
    }

    _events.clear();
}

bool
EventListener::terminate(void) const
{
    return _terminate;
}

void
EventListener::terminate(const bool value)
{
    _terminate = value;
}

