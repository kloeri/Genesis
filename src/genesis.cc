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

#include <cstring>
#include <limits.h>
#include <map>
#include <pcre++.h>
#include <string>
#include <sys/mount.h>

#include <actions/action.hh>
#include <logger.hh>
#include <events/event.hh>
#include <events/netlink-uevent.hh>
#include <events/netlink-route.hh>
#include <events/genesis-fifo.hh>
#include <utils.hh>

namespace
{
    const std::string GenesisVersion = "0.1alpha0";
}

class EventListener
{
    private:
        std::map<int, EventManager *> eventmanagers;

    public:
        void add_eventsource(EventManager * eventmanager);
        void listen();
        void send_event(std::string event);
};

void EventListener::add_eventsource(EventManager * eventmanager)
{
    int fd = eventmanager->get_fd();
    eventmanagers[fd] = eventmanager;
}

void EventListener::listen()
{
    int maxfd = 0;
    fd_set readfds;
    FD_ZERO(&readfds);

    for (std::map<int, EventManager *>::iterator iter = eventmanagers.begin(); iter != eventmanagers.end(); ++iter)
    {
        FD_SET(iter->first, &readfds);
        maxfd = iter->first > maxfd ? iter->first : maxfd;
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
                Logger::get_instance()->Log(INFO, action->Identity());
                Logger::get_instance()->Log(DEBUG, "Result of Execute(): " + action->GetResult());
                send_event(action->Identity());
            }
        }
    }
}

void EventListener::send_event(std::string event)
{
    for (std::map<int, EventManager *>::iterator iter = eventmanagers.begin(); iter != eventmanagers.end(); ++iter)
    {
        iter->second->new_event(event);
    }
}

int main(int argc, char * argv[])
{
    WelcomeBlurb();

    Logger::get_instance()->set_log_level(DEBUG);

    EventListener listener;
    listener.add_eventsource(new GenesisFIFO());
    listener.add_eventsource(new NetlinkUevent());
    listener.add_eventsource(new NetlinkRoute());
    listener.send_event("genesis-initialising");
    listener.send_event("genesis-started");
    while (true)
    {
        listener.listen();
    }
}

