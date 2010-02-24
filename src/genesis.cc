/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2009 Bryan Østergaard
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
#include <events/event.hh>
#include <events/netlink-uevent.hh>
#include <events/netlink-route.hh>
#include <events/genesis-fifo.hh>
#include <config.hh>
#include <utils.hh>

namespace
{
    using namespace genesis;
    const std::string GenesisVersion = "0.1alpha0";
}

class EventListener
{
    private:
        std::map<int, EventManager *> eventmanagers;

    public:
        void add_eventsource(EventManager * eventmanager);
        void listen();
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
            }
        }
    }
}

int main(int argc, char * argv[])
{
    WelcomeBlurb();

    // Read config file
    genesis::Configuration ModulesConfiguration(SYSCONFDIR "config", "modules");
    genesis::Configuration GenesisConfiguration(SYSCONFDIR "config", "genesis");

    // Open log file
    std::ofstream Logfile;
    if (GenesisConfiguration.get_option("logging") == "file")
    {
        Logfile.open(GenesisConfiguration.get_option("logfile").c_str());
    }

    EventListener listener;
    listener.add_eventsource(new GenesisFIFO());
    listener.add_eventsource(new NetlinkUevent());
    listener.add_eventsource(new NetlinkRoute());
    while (true)
    {
        listener.listen();
    }

    return 0;
}

