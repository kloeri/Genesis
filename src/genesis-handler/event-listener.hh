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

#ifndef SRC_GENESIS_HANDLER_EVENT_LISTENER_HH
#define SRC_GENESIS_HANDLER_EVENT_LISTENER_HH

#include <map>
#include <string>

#include <events/event.hh>

class EventListener
{
    private:
        std::map<int, EventManager *> eventmanagers;
        std::list<std::string> eventqueue;

        void send_event(std::string event);

    public:
        ~EventListener(void);

        void add_eventsource(EventManager * eventmanager);
        void add_event(std::string event);
        void listen();
        void process_eventqueue();
};

#endif

