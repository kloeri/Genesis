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

#include <string>

#include <logger.hh>
#include <actions/action.hh>

#include <event-listener.hh>

EventListener::~EventListener(void)
{
    std::map<int, EventManager *>::iterator manager;

    for (manager = eventmanagers.begin(); manager != eventmanagers.end(); ++manager)
    {
        delete manager->second;
    }
}

void
EventListener::add_eventsource(EventManager * eventmanager)
{
    int fd = eventmanager->get_fd();
    eventmanagers[fd] = eventmanager;

    // Grab all 'coldplug' events
    std::list<std::string> coldplug_events = eventmanager->get_events();
    eventqueue.splice(eventqueue.end(), coldplug_events);
}

void EventListener::add_event(std::string event)
{
    eventqueue.push_back(event);
}

void
EventListener::listen()
{
    int maxfd = 0;
    fd_set readfds;
    FD_ZERO(&readfds);

    for (std::map<int, EventManager *>::iterator iter = eventmanagers.begin(); iter != eventmanagers.end(); ++iter)
    {
        if (iter->first >= 0)
        {
            FD_SET(iter->first, &readfds);
            maxfd = iter->first > maxfd ? iter->first : maxfd;
        }
    }
    select(maxfd + 1, &readfds, NULL, NULL, NULL);
    for (std::map<int, EventManager *>::iterator iter = eventmanagers.begin(); iter != eventmanagers.end(); ++iter)
    {
        if FD_ISSET(iter->first, &readfds)
        {
            Action * action = iter->second->GetEvent();
            if (action != NULL)
            {
                action->Execute();
                Logger::get_instance().Log(INFO, action->Identity());
                Logger::get_instance().Log(DEBUG, "Result of Execute(): " + action->GetResult());
                send_event(action->Identity());
            }
        }
    }
}

void
EventListener::process_eventqueue()
{
    for (std::list<std::string>::iterator event = eventqueue.begin(); event != eventqueue.end(); ++event)
    {
        for (std::map<int, EventManager *>::iterator iter = eventmanagers.begin(); iter != eventmanagers.end(); ++iter)
        {
            Action * action = iter->second->new_event(*event);
            if (action != NULL)
            {
                action->Execute();
                Logger::get_instance().Log(INFO, action->Identity());
                Logger::get_instance().Log(DEBUG, "Result of Execute(): " + action->GetResult());
                send_event(action->Identity());
            }
        }
    }
    eventqueue.clear();
}

void
EventListener::send_event(std::string event)
{
    for (std::map<int, EventManager *>::iterator iter = eventmanagers.begin(); iter != eventmanagers.end(); ++iter)
    {
        Action * action = iter->second->new_event(event);
        if (action != NULL)
        {
            action->Execute();
            Logger::get_instance().Log(INFO, action->Identity());
            Logger::get_instance().Log(DEBUG, "Result of Execute(): " + action->GetResult());
            send_event(action->Identity());
        }
    }
}

