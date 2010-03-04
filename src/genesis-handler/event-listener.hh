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
#include <list>
#include <string>

#include "event-sources/event.hh"
#include "event-sources/event-source.hh"

namespace genesis
{
    class EventListener
    {
        private:
            std::map<int, EventManager *> _managers;
            std::map<int, genesis::events::EventSource *> _sources;
            std::list<std::string> _events;

        public:
            ~EventListener(void);

            void add_source(genesis::events::EventSource *source);

            void add_eventsource(EventManager *manager);
            void add_event(const std::string & event);
            void listen(void);
            void process_eventqueue(void);
    };
}

#endif

