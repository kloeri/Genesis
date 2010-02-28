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
#include <iostream>

#include <logger.hh>
#include <event-listener.hh>
#include <event-sources/genesis-fifo.hh>
#include <event-sources/netlink-route.hh>
#include <event-sources/netlink-uevent.hh>

using namespace genesis;

int main(int argc, char * argv[])
{
    Logger logger;
    EventListener listener;

    std::cout << "Genesis (c) 2010 Bryan Østergaard <kloeri@exherbo.org>" << std::endl;

    logger.set_log_level(DEBUG);

    listener.add_eventsource(new GenesisFIFO());
    listener.add_eventsource(new NetlinkUevent());
    listener.add_eventsource(new NetlinkRoute());
    listener.add_event("genesis-initialising");
    listener.add_event("genesis-started");
    listener.process_eventqueue();

    while (true)
    {
        listener.listen();
    }
}

