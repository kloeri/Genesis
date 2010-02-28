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

#ifndef SRC_GUARD_EVENTS_GENESIS_FIFO_HH
#define SRC_GUARD_EVENTS_GENESIS_FIFO_HH 1

#include <fstream>
#include <list>
#include <string>
#include <actions/action.hh>
#include <events/event.hh>

class GenesisFIFO : public EventManager
{
    private:
        int fd;

    public:
        // Sets up the /dev/genesis fifo / command channel
        GenesisFIFO();
        ~GenesisFIFO();

        std::list<std::string> get_events();

        // Listens for data on /dev/genesis fifo and sends Actions to genesis proper
        Action * GetEvent();
        int get_fd();
        Action * new_event(std::string event);
};

#endif
