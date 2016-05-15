/* vim: set sw=2 sts=2 et foldmethod=syntax : */
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

#include <iostream>
#include <string>

#include "util/log.hh"

#include <event-sources/netlink-route.hh>
#include <event-sources/netlink-uevent.hh>

#include "event-sources/genesis-pipe.hh"
#include "genesis-handler/event-listener.hh"
//#include "jobs/jobs.hh"

using namespace genesis;
using namespace genesis::events;
using namespace genesis::logging;

int main(int argc, char *argv[]) {
  Log log;
  EventListener listener;

  std::cout << "Genesis (c) 2010 Bryan Østergaard <kloeri@exherbo.org>"
            << std::endl;

  // Main configuration
  Configuration *GenesisConfiguration;
  std::map<std::string, std::string> defaultconfig;
  defaultconfig["loglevel"] = "debug";

  GenesisConfiguration =
      new Configuration(SYSCONFDIR "genesis.conf", "genesis", defaultconfig);
  if (GenesisConfiguration->get_option("loglevel") == "debug") {
    log.set_minimum_log_level(DEBUG);
  }
  if (GenesisConfiguration->get_option("loglevel") == "info") {
    log.set_minimum_log_level(INFO);
  }
  if (GenesisConfiguration->get_option("loglevel") == "notice") {
    log.set_minimum_log_level(NOTICE);
  }
  if (GenesisConfiguration->get_option("loglevel") == "warn") {
    log.set_minimum_log_level(WARN);
  }
  if (GenesisConfiguration->get_option("loglevel") == "crit") {
    log.set_minimum_log_level(CRIT);
  }
  if (GenesisConfiguration->get_option("loglevel") == "err") {
    log.set_minimum_log_level(ERR);
  }

  // Jobs configuration
//  JobsConfiguration *Jobs;
//
//  Jobs = new JobsConfiguration(SYSCONFDIR "jobs.conf");

  listener.add_source(new GenesisPipe);

  listener.add_eventsource(new NetlinkUevent());
  listener.add_eventsource(new NetlinkRoute());
  //listener.add_event("genesis-initialising");
  //listener.add_event("genesis-started");

  while (!listener.terminate()) {
    listener.process_eventqueue();
    listener.listen();
  }
}
