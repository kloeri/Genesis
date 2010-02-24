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

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <netlink-route.hh>
#include <actions/bash-action.hh>

namespace
{
//    std::list<std::string> events;

    int SourceScriptsCallback(const struct dirent * entry)
    {
        if (strlen(entry->d_name) > 3)
        {
            if (std::strcmp(entry->d_name + std::strlen(entry->d_name) - 3, ".sh") == 0)
            {
                return 1;
            }
        }
        return 0;
    }
}

//
// Open sockets for all supported netlink protocols
//
NetlinkRoute::NetlinkRoute()
{
    SourceScripts(SYSCONFDIR "netlink-route/");
    OpenSocket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE, RTNLGRP_MAX);
}

void NetlinkRoute::SourceScripts(std::string path)
{
    struct dirent **eps;
    int n = scandir(path.c_str(), &eps, SourceScriptsCallback, alphasort);
    if (n >= 0)
    {
        for (int cnt = 0; cnt < n; ++cnt)
        {
            std::string scriptfile(SYSCONFDIR "netlink-route/");
            scriptfile += eps[cnt]->d_name;

            BashAction * action = new BashAction("get-metadata", scriptfile);
            action->Execute();

            std::stringstream envvars(action->GetResult());
            while (envvars.good())
            {
                std::string line;
                std::getline(envvars, line);
                if (line != "")
                {
                    int delimiter = line.find(", ");
                    std::string function(line.substr(0, delimiter));
                    std::string match(line.substr(delimiter + 2, line.size()));
                    eventsubscriptions.push_back(eventhandler(scriptfile, function, pcrepp::Pcre(match)));
                }
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open the directory" << std::endl;
    }
}

//
// Supported protocols are:
// NETLINK_ROUTE
//
void * NetlinkRoute::OpenSocket(int domain, int type, int protocol, int multicastgroup)
{
    sockaddr_nl source_address;

    if ((netlinksocket = socket(domain, type, protocol)) == -1)
    {
        std::perror("socket");
        return 0;
    }

    memset(&source_address, 0, sizeof(source_address));
    source_address.nl_family = AF_NETLINK;
    source_address.nl_pid = getpid();  /* self pid */
    source_address.nl_groups = multicastgroup;
    if (bind(netlinksocket, (struct sockaddr *) &source_address, sizeof(struct sockaddr_nl)) == -1)
    {
        std::perror("bind");
        return 0;
    }

    int on = 1;
    if (setsockopt(netlinksocket, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on)) == -1)
    {
        std::perror("setsockopt");
        return 0;
    }
    return NULL;
}

NetlinkRoute::~NetlinkRoute()
{
    /* Close Netlink Socket */
    close(netlinksocket);
}

void NetlinkRoute::serialize_ifa_flags(std::ostringstream & event, int flags)
{
    event << ";G_ADDRESS_FLAGS=";
    if (flags & 0x01)
        event << "secondary_address";
    if (flags & 0x02)
        event << "no_dad_(duplicate_address_discovery)";
    if (flags & 0x04)
        event << "optimistic_(optimistic_duplicate_address_discovery)";
    if (flags & 0x10)
        event << "home address";
    if (flags & 0x20)
        event << "deprecated";
    if (flags & 0x40)
        event << "tentative_address";
    if (flags & 0x80)
        event << "permanent_address";
}

void NetlinkRoute::serialize_ifa_family(std::ostringstream & event, int family)
{
    event << ";G_ADDRESS_FAMILY=";
    switch (family)
    {
        case AF_INET:
            event << "AF_INET";
            break;
        case AF_INET6:
            event << "AF_INET6";
            break;
        default:
            event << "unknown";
    }
}

void NetlinkRoute::serialize_rtm_newaddr(std::ostringstream & event, nlmsghdr * header)
{
    struct ifaddrmsg * ifaddrmsg_payload;

    event << "G_EVENTTYPE=NEW_IP_ADDRESS";
    ifaddrmsg_payload = (struct ifaddrmsg *) NLMSG_DATA(header);
    event << ";ADDRESS_FAMILY=";
    serialize_ifa_family(event, ifaddrmsg_payload->ifa_family);
    event << ";PREFIX_LENGTH=" << static_cast<int>(ifaddrmsg_payload->ifa_prefixlen);
    serialize_ifa_flags(event, ifaddrmsg_payload->ifa_flags);
    event <<  ";ADDRESS_SCOPE=" << static_cast<int>(ifaddrmsg_payload->ifa_scope);
    event << ";INTERFACE_INDEX=" << ifaddrmsg_payload->ifa_index;
}

void NetlinkRoute::serialize_rtm_deladdr(std::ostringstream & event, nlmsghdr * header)
{
    struct ifaddrmsg * ifaddrmsg_payload;

    event << "removed_ip_address";
    ifaddrmsg_payload = (struct ifaddrmsg *) NLMSG_DATA(header);
    serialize_ifa_family(event, ifaddrmsg_payload->ifa_family);
    event << ";G_PREFIX_LENGTH=" << static_cast<int>(ifaddrmsg_payload->ifa_prefixlen);
    event << ";G_ADDRESS_FLAGS=";
    serialize_ifa_flags(event, ifaddrmsg_payload->ifa_flags);
    event << ";G_ADDRESS_SCOPE=" << static_cast<int>(ifaddrmsg_payload->ifa_scope);
    event << ";G_INTERFACE_INDEX=" << ifaddrmsg_payload->ifa_index;
}

void NetlinkRoute::serialize_rtm_table(std::ostringstream & event, int table)
{
    switch (table)
    {
        case RT_TABLE_UNSPEC:
            event << "unspecified_routing_table";
            break;
        case RT_TABLE_DEFAULT:
            event << "default_routing_table";
            break;
        case RT_TABLE_MAIN:
            event << "main_routing_table";
            break;
        case RT_TABLE_LOCAL:
            event << "local_routing_table";
            break;
        default:
            event << "unknown";
            break;
    }
}

void NetlinkRoute::serialize_rtm_protocol(std::ostringstream & event, int protocol)
{
    switch (protocol)
    {
        case RTPROT_UNSPEC:
            event << "unknown";
            break;
        case RTPROT_REDIRECT:
            event << "icmp_redirect";
            break;
        case RTPROT_KERNEL:
            event << "kernel";
            break;
        case RTPROT_BOOT:
            event << "boot";
            break;
        case RTPROT_STATIC:
            event << "static";
    }
}

void NetlinkRoute::serialize_rtm_scope(std::ostringstream & event, int scope)
{
    switch (scope)
    {
        case RT_SCOPE_UNIVERSE:
            event << "global_route";
            break;
        case RT_SCOPE_SITE:
            event << "interior_route";
            break;
        case RT_SCOPE_LINK:
            event << "route_on_this_link";
            break;
        case RT_SCOPE_HOST:
            event << "route_on_the_local_host";
            break;
        case RT_SCOPE_NOWHERE:
            event << "destination_does_not_exist";
            break;
        default:
            event << "unknown_scope";
            break;
    }
}

void NetlinkRoute::serialize_rtm_type(std::ostringstream & event, int type)
{
    switch (type)
    {
        case RTN_UNSPEC:
            event << "unknown_route";
            break;
        case RTN_UNICAST:
            event << "unicast_route";
            break;
        case RTN_LOCAL:
            event << "local_interface_route";
            break;
        case RTN_BROADCAST:
            event << "local_broadcast_route";
            break;
        case RTN_ANYCAST:
            event << "local_broadcast_route_(sent_as_a_unicast_route)";
            break;
        case RTN_MULTICAST:
            event << "multicast_route";
            break;
        case RTN_BLACKHOLE:
            event << "blackhole_route";
            break;
        case RTN_UNREACHABLE:
            event << "unreachable_destination";
            break;
        case RTN_PROHIBIT:
            event << "packet_rejection_route";
            break;
        case RTN_THROW:
            event << "continue_routing_lookup_in_another_table";
            break;
        case RTN_NAT:
            event << "network_address_translation_rule";
            break;
        case RTN_XRESOLVE:
            event << "refer_to_external_resolver_(not_implemented)";
            break;
    }
}

void NetlinkRoute::serialize_rtm_flags(std::ostringstream & event, int flags)
{
    event << ";G_ROUTING_FLAGS=";
    if (flags & RTM_F_NOTIFY)
        event << "motify_user_of_routing_changes";
    if (flags & RTM_F_CLONED)
        event << "route_is_cloned_from_another_route";
    if (flags & RTM_F_EQUALIZE)
        event << "multipath_equalizer";
}

void NetlinkRoute::serialize_ndm_family(std::ostringstream & event, int family)
{
    event << ";G_FAMILY=";
    switch (family)
    {
        case AF_INET:
            event << "AF_INET";
            break;
        case AF_INET6:
            event << "AF_INET6";
            break;
        default:
            event << "unknown_address_family";
            break;
    }
}

void NetlinkRoute::serialize_ndm_state(std::ostringstream & event, int state)
{
    event << ";G_STATE=";
    switch (state)
    {
        case NUD_INCOMPLETE:
            event << "currently_resolving_cache_entry";
            break;
        case NUD_REACHABLE:
            event << "confirmed_working_cache_entry";
            break;
        case NUD_STALE:
            event << "expired_cache_entry";
            break;
        case NUD_DELAY:
            event << "entry_waiting_for_a_timer";
            break;
        case NUD_PROBE:
            event << "cache_entry_currently_being_reprobed";
            break;
        case NUD_FAILED:
            event << "invalid_cache_entry";
            break;
        case NUD_NOARP:
            event << "device_with_no_destination_cache";
            break;
        case NUD_PERMANENT:
            event << "static_cache_entry";
            break;
        default:
            event << "unknown_state";
            break;
    }
}

void NetlinkRoute::serialize_ndm_flags(std::ostringstream & event, int flags)
{
    event << ";G_FLAGS=" << flags;
    if (flags & NTF_PROXY)
        event << "proxy_arp_entry";
    if (flags & NTF_ROUTER)
        event << "ipv6_router";
}

Action *NetlinkRoute::GetEvent()
{
    std::ostringstream event_strings;
    char netlink_buffer[PIPE_BUF];
    memset(netlink_buffer, 0, PIPE_BUF);

    struct msghdr msg;
    struct iovec iov;
    char cred_msg[CMSG_SPACE(20)];

    // Clear data structures
    memset(&msg, 0, sizeof(struct msghdr));
    memset(&iov, 0, sizeof(struct iovec));
    memset(&cred_msg, 0, sizeof(cred_msg));

    // Setup scatter-gather structure
    iov.iov_base = &netlink_buffer;
    iov.iov_len = sizeof(netlink_buffer);

    // Setup message structure
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = cred_msg;
    msg.msg_controllen = sizeof(cred_msg);

    size_t len = recvmsg(netlinksocket, &msg, 0);
    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg == NULL || cmsg->cmsg_type != SCM_CREDENTIALS)
        return NULL;
    struct ucred * cred = reinterpret_cast<ucred *> CMSG_DATA(cmsg);
    if (cred->uid != 0)
        return NULL;

    for (struct nlmsghdr * netlink_header = (struct nlmsghdr *) netlink_buffer; NLMSG_OK(netlink_header, len); netlink_header = NLMSG_NEXT(netlink_header, len))
    {
        switch(netlink_header->nlmsg_type)
        {
            case NLMSG_DONE:
                break;
            case NLMSG_ERROR:
                break;

                // Link state
            case RTM_NEWLINK:
                event_strings << "new_link";
                break;
            case RTM_DELLINK:
                event_strings << "removed_link";
                break;
            case RTM_SETLINK:
                event_strings << "set_link";
                break;

                // IP address management
            case RTM_NEWADDR:
                serialize_rtm_newaddr(event_strings, netlink_header);
                break;
            case RTM_DELADDR:
                serialize_rtm_deladdr(event_strings, netlink_header);
                break;

                // Route changes
            case RTM_NEWROUTE:
                struct rtmsg * rtmsg_payload;

                event_strings << "new_route";
                rtmsg_payload = (struct rtmsg *) NLMSG_DATA(netlink_header);
                event_strings << ";G_ADDRESS_FAMILY=" << static_cast<int>(rtmsg_payload->rtm_family);
                event_strings << ";g_DESTINATION_LENGTH=" << static_cast<int>(rtmsg_payload->rtm_dst_len);
                event_strings << ";g_SOURCE_LENGTH=" << static_cast<int>(rtmsg_payload->rtm_src_len);
                event_strings << ";G_TOS_FILTER=" << static_cast<int>(rtmsg_payload->rtm_tos);
                event_strings << ";G_ROUTING_TABLE_ID=" << static_cast<int>(rtmsg_payload->rtm_table);
                serialize_rtm_table(event_strings, rtmsg_payload->rtm_table);
                event_strings << ";G_ROUTING_PROTOCOL=" << static_cast<int>(rtmsg_payload->rtm_protocol);
                serialize_rtm_protocol(event_strings, rtmsg_payload->rtm_protocol);
                event_strings << ";G_ROUTING_SCOPE=" << static_cast<int>(rtmsg_payload->rtm_scope);
                serialize_rtm_scope(event_strings, rtmsg_payload->rtm_scope);
                event_strings << ";G_ROUTING_TYPE=" << static_cast<int>(rtmsg_payload->rtm_type);
                serialize_rtm_type(event_strings, rtmsg_payload->rtm_type);
                serialize_rtm_flags(event_strings, rtmsg_payload->rtm_flags);
                break;
            case RTM_DELROUTE:
                event_strings << "removed_route";
                rtmsg_payload = (struct rtmsg *) NLMSG_DATA(netlink_header);
                event_strings << ";G_ADDRESS_FAMILY=" << static_cast<int>(rtmsg_payload->rtm_family);
                event_strings << ";G_DESTINATION_LENGTH=" << static_cast<int>(rtmsg_payload->rtm_dst_len);
                event_strings << ";G_SOURCE_LENGTH=" << static_cast<int>(rtmsg_payload->rtm_src_len);
                event_strings << ";G_TOS_FILTER=" << static_cast<int>(rtmsg_payload->rtm_tos);
                event_strings << ";G_ROUTING_TABLE_ID=" << static_cast<int>(rtmsg_payload->rtm_table);
                event_strings << ";G_ROUTING_PROTOCOL=" << static_cast<int>(rtmsg_payload->rtm_protocol);
                event_strings << ";G_ROUTING_SCOPE=" << static_cast<int>(rtmsg_payload->rtm_scope);
                event_strings << ";G_ROUTING_TYPE=" << static_cast<int>(rtmsg_payload->rtm_type);
                event_strings << ";G_ROUTING_FLAGS=" << static_cast<int>(rtmsg_payload->rtm_flags);
                break;

                // Neightborhood discovery (ARP)
            case RTM_NEWNEIGH:
                event_strings << "G_EVENTTYPE=new_neighbor;";
                struct ndmsg * ndmsg_payload;
                ndmsg_payload = (struct ndmsg *) NLMSG_DATA(netlink_header);
                serialize_ndm_family(event_strings, ndmsg_payload->ndm_family);
                event_strings << ";G_INTERFACE=" << static_cast<int>(ndmsg_payload->ndm_ifindex);
                serialize_ndm_state(event_strings, ndmsg_payload->ndm_state);
                serialize_ndm_flags(event_strings, ndmsg_payload->ndm_flags);
                event_strings << ";G_TYPE=" << static_cast<int>(ndmsg_payload->ndm_type);
                break;
            case RTM_DELNEIGH:
                event_strings << "G_EVENTTYPE=removed_neighbor;";
                ndmsg_payload = (struct ndmsg *) NLMSG_DATA(netlink_header);
                serialize_ndm_family(event_strings, ndmsg_payload->ndm_family);
                event_strings << ";G_INTERFACE=" << static_cast<int>(ndmsg_payload->ndm_ifindex);
                serialize_ndm_state(event_strings, ndmsg_payload->ndm_state);
                serialize_ndm_flags(event_strings, ndmsg_payload->ndm_flags);
                event_strings << ";G_TYPE=" << static_cast<int>(ndmsg_payload->ndm_type);
                break;
            default:
                event_strings << ";unknown_netlink_event_received=" << netlink_header->nlmsg_type;
                break;
        }
    }
    event_strings << std::endl;
    std::string return_val("G_EVENTSOURCE=route;");
    return_val += event_strings.str();
    return ProcessEvent(return_val);
}

Action * NetlinkRoute::ProcessEvent(std::string event)
{
    for (std::list<eventhandler>::iterator iter = eventsubscriptions.begin(); iter != eventsubscriptions.end(); ++iter)
    {
        if (iter->match.search(event))
        {
            pcrepp::Pcre splitregex(";", "g");
            return new BashAction("run-function", iter->filename, iter->function, splitregex.split(event));
        }
    }
    return 0;
}

int NetlinkRoute::get_fd()
{
    return netlinksocket;
}
