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

#ifndef EVENTS_GUARD_NETLINK_UEVENT_HH
#define EVENTS_GUARD_NETLINK_UEVENT_HH 1

#include <list>
#include <utility>
#include <config.hh>
#include <events/event.hh>

class NetlinkUevent : public EventManager
{
    private:
        Configuration * UEventConfiguration;
        std::list<eventhandler> eventsubscriptions;
        int netlinksocket;

        // Generate a list of coldplug events
        void GenerateEvents();

        // Actual method responsible for the netlink uevent socket setup
        void * OpenSocket(int domain, int type, int protocol, int multicastgroup);

        // Iterates over netlink-uevent scripts and gathers SUBSCRIPTION_* metadata
        void SourceScripts(std::string path);

    public:
        // Sets up default configuration, does coldplugging if wanted and sets up the netlink uevent socket
        NetlinkUevent();
        ~NetlinkUevent();

        // Return all queued events (think coldplugging) and clear the internal queue
        std::list<std::string> get_events();

        // Handles raw events, matching them to subscriptions and sending Actions to genesis proper as needed
        Action * ProcessEvent(std::string event);

        // Listens for netlink uevent messages and sends raw events to ProcessEvent
        Action * GetEvent();

        int get_fd();
        Action * new_event(std::string event);
};

#endif
