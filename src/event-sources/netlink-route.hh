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

#ifndef EVENTS_GUARD_NETLINK_ROUTE_HH
#define EVENTS_GUARD_NETLINK_ROUTE_HH 1

#include "genesis-handler/config.hh"
#include <actions/action.hh>
#include <event-sources/event.hh>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <list>
#include <pcre++.h>
#include <string>
#include <sys/socket.h>
#include <utility>

class NetlinkRoute : public EventManager {
private:
  Configuration *URouteConfiguration;
  int netlinksocket;
  std::list<eventhandler> eventsubscriptions;

  void SourceScripts(std::string path);
  void *OpenSocket(int domain, int type, int protocol, int multicastgroup);
  void serialize_rtm_newaddr(std::ostringstream &event, nlmsghdr *header);
  void serialize_rtm_deladdr(std::ostringstream &event, nlmsghdr *header);
  void serialize_ifa_family(std::ostringstream &event, int family);
  void serialize_ifa_flags(std::ostringstream &event, int flags);
  void serialize_rtm_table(std::ostringstream &event, int table);
  void serialize_rtm_protocol(std::ostringstream &event, int protocol);
  void serialize_rtm_scope(std::ostringstream &event, int scope);
  void serialize_rtm_type(std::ostringstream &event, int type);
  void serialize_rtm_flags(std::ostringstream &event, int flags);
  void serialize_ndm_family(std::ostringstream &event, int family);
  void serialize_ndm_state(std::ostringstream &event, int state);
  void serialize_ndm_flags(std::ostringstream &event, int flags);

public:
  NetlinkRoute();
  ~NetlinkRoute();
  std::list<std::string> get_events();
  Action *GetEvent();
  Action *ProcessEvent(std::string event);
  int get_fd();
  Action *new_event(std::string event);
};

#endif
