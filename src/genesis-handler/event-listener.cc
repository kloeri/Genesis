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
#include "genesis-handler/logger.hh"

#define EVENT_LISTENER          ("Event-Listener")

using namespace genesis;

EventListener::~EventListener(void)
{
    std::map<int, EventManager *>::iterator manager;

    for (manager = _managers.begin(); manager != _managers.end(); ++manager)
    {
        delete manager->second;
    }
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

    for (manager = _managers.begin(); manager != _managers.end(); ++manager)
    {
        if (manager->first >= 0)
        {
            FD_SET(manager->first, &readfds);
            maxfd = std::max(manager->first, maxfd);
        }
    }

    ::select(maxfd + 1, &readfds, NULL, NULL, NULL);

    for (manager = _managers.begin(); manager != _managers.end(); ++manager)
    {
        if (FD_ISSET(manager->first, &readfds))
        {
            std::unique_ptr<Action> action(manager->second->GetEvent());

            if (action.get())
            {
                Logger::get_instance().log(INFO, EVENT_LISTENER, "received action: " + action->Identity());
                action->Execute();
                Logger::get_instance().log(DEBUG, EVENT_LISTENER, "action result: " + action->GetResult());

                send_event(action->Identity());
            }
        }
    }
}

void
EventListener::process_eventqueue(void)
{
    std::list<std::string>::const_iterator event;
    std::map<int, EventManager *>::const_iterator manager;

    for (event = _events.begin(); event != _events.end(); ++event)
    {
        for (manager = _managers.begin(); manager != _managers.end(); ++manager)
        {
            std::unique_ptr<Action> action(manager->second->new_event(*event));

            if (action.get())
            {
                Logger::get_instance().log(INFO, EVENT_LISTENER, "received action: " + action->Identity());
                action->Execute();
                Logger::get_instance().log(DEBUG, EVENT_LISTENER, "action result: " + action->GetResult());

                send_event(action->Identity());
            }
        }
    }

    _events.clear();
}

void
EventListener::send_event(const std::string & event)
{
    std::map<int, EventManager *>::const_iterator manager;

    for (manager = _managers.begin(); manager != _managers.end(); ++manager)
    {
        std::unique_ptr<Action> action(manager->second->new_event(event));

        if (action.get())
        {
            Logger::get_instance().log(INFO, EVENT_LISTENER, "received action: " + action->Identity());
            action->Execute();
            Logger::get_instance().log(DEBUG, EVENT_LISTENER, "action result: " + action->GetResult());

            send_event(action->Identity());
        }
    }
}

