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

#ifndef SRC_GUARD_EVENTS_EVENT_HH
#define SRC_GUARD_EVENTS_EVENT_HH 1

#include <list>
#include <string>
#include <pcre++.h>
#include <actions/action.hh>

struct eventhandler
{
    eventhandler(std::string fn, std::string f, pcrepp::Pcre m) : filename(fn), function(f), match(m) {}
    std::string filename;
    std::string function;
    pcrepp::Pcre match;
};

class EventManager
{
    public:
	virtual std::list<std::string> get_events() = 0;
        virtual Action * GetEvent() = 0;
        virtual int get_fd() = 0;
        virtual ~EventManager() {}
        virtual Action * new_event(std::string) = 0;
};

#endif
