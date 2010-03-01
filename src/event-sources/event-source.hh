/* vim: set sw=4 sts=4 et foldmethod=syntax : */
/*
 * Copyright © 2010 Saleem Abdulrasool
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

#ifndef SRC_EVENT_SOURCES_EVENT_SOURCE_HH
#define SRC_EVENT_SOURCES_EVENT_SOURCE_HH

#include <memory>

#include "actions/action.hh"

namespace genesis
{
    namespace events
    {
        class EventSource
        {
            protected:
                int _fd;

                EventSource(void);

            public:
                virtual ~EventSource(void);

                virtual int fd(void) const;

                virtual std::unique_ptr<Action> process_event(void) = 0;
                virtual void event_processed(const std::string & event) = 0;
        };
    }
}

#endif

