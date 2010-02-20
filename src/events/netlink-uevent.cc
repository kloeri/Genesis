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
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <ftw.h>
#include <fstream>
#include <iostream>
#include <sys/socket.h>                       // Include before linux/{,rt}netlink.h
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <pcre++.h>
#include <sstream>
#include <string>
#include <sys/mount.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <bash.hh>
#include <config.hh>
#include <netlink-uevent.hh>
#include <eventnotifier.hh>
#include <actions/bash-action.hh>

namespace
{
    std::list<std::string> events;

    // Filter functions used by ftw() during coldplugging and metadata sourcing
    int GenerateEventsCallback(const char *name, const struct stat *filestat, int flags, struct FTW *ftwbuf)
    {
        std::string eventname(name);

        if (filestat->st_mode & S_IRUSR)
        {
            if ((eventname.length() >= 7) && (eventname.compare(eventname.length() - 7, 7, "/uevent") == 0))
            {
                // Add event specific data
                std::ifstream eventdata(eventname.c_str());

                // Remove /uevent part
                eventname.erase(eventname.length() - 7, 7);

                while (!eventdata.eof())
                {
                    std::string line;
                    std::getline(eventdata, line);
                    if (line != "")
                    {
                        eventname += ";G_" + line;
                    }
                }

                // We don't care about /sys but coldplug events should start with add@ to signify adding new devices
                eventname.replace(0, 4, "add@");

                events.push_back(eventname);
            }
        }
        return FTW_CONTINUE;
    }

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

void NetlinkUevent::GenerateEvents()
{
    if ((UEventConfiguration->get_option("coldplug") == "yes") && (UEventConfiguration->get_option("coldplug_mounts_sysfs") == "yes"))
    {
        if (mount("none", "/sys", "sysfs", 9, NULL) == -1)
        {
            std::perror("mount");
        }
    }

    int retcode = 0;
    retcode = nftw("/sys/dev/", GenerateEventsCallback, 1, FTW_ACTIONRETVAL);

    if ((UEventConfiguration->get_option("coldplug") == "yes") && (UEventConfiguration->get_option("coldplug_mounts_sysfs") == "yes"))
    {
        if (umount("/sys") == -1)
        {
            std::perror("unmount");
        }
    }
}

void NetlinkUevent::EmitEvents()
{
    for (std::list<std::string>::const_iterator iter = events.begin(); iter != events.end(); ++iter)
    {
        ProcessEvent(*iter);
    }
}

//
// Open sockets for all supported netlink protocols
//
NetlinkUevent::NetlinkUevent(genesis::EventNotifier * notify)
{
    _notify = notify;
    std::map<std::string, std::string> defaultconfig;
    defaultconfig["coldplug"] = "no";
    defaultconfig["log_matched_events"] = "no";
    defaultconfig["log_unmatched_events"] = "yes";
    defaultconfig["coldplug_mount_sysfs"] = "yes";

    UEventConfiguration = new genesis::Configuration(SYSCONFDIR "config", "netlink-uevent", defaultconfig);
    SourceScripts(SYSCONFDIR "netlink-uevent");

    if (UEventConfiguration->get_option("coldplug") == "yes")
    {
        GenerateEvents();
        EmitEvents();
    }

    OpenSocket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT, 1);
}

void NetlinkUevent::SourceScripts(std::string path)
{
    struct dirent **eps;
    int n = scandir(path.c_str(), &eps, SourceScriptsCallback, alphasort);
    if (n >= 0)
    {
        for (int cnt = 0; cnt < n; ++cnt)
        {
            std::string scriptfile(SYSCONFDIR "netlink-uevent/");
            scriptfile += eps[cnt]->d_name;

            BashAction * action = new BashAction("get-metadata", scriptfile);
            _notify->lock();
            _notify->setaction(action);
            _notify->signal();
            _notify->wait();
            _notify->unlock();

            std::stringstream envvars(_notify->getaction()->GetResult());
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
// NETLINK_KOBJECT_UEVENT
// NETLINK_ROUTE
//
void * NetlinkUevent::OpenSocket(int domain, int type, int protocol, int multicastgroup)
{
    sockaddr_nl source_address;

    if ((netlinksocket = socket(domain, type, protocol)) == -1)
    {
        std::perror("socket");
        pthread_exit((void *) -1);
    }

    memset(&source_address, 0, sizeof(source_address));
    source_address.nl_family = AF_NETLINK;
    source_address.nl_pid = getpid();  /* self pid */
    source_address.nl_groups = multicastgroup;
    if (bind(netlinksocket, (struct sockaddr *) &source_address, sizeof(struct sockaddr_nl)) == -1)
    {
        std::perror("bind");
        pthread_exit((void *) -1);
    }

    int on = 1;
    if (setsockopt(netlinksocket, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on)) == -1)
    {
        std::perror("setsockopt");
        pthread_exit((void *) -1);
    }
    return NULL;
}

NetlinkUevent::~NetlinkUevent()
{
    close(netlinksocket);
}

void NetlinkUevent::ProcessEvent(std::string event)
{
    if (event.find("add@") != std::string::npos)
        event.replace(event.find("add@"), 4, "G_EVENTTYPE=newdevice;G_DEVICE=");
    if (event.find("remove@") != std::string::npos)
        event.replace(event.find("remove@"), 7, "G_EVENTTYPE=removedevice;G_DEVICE=");
    if (event.find("change@") != std::string::npos)
        event.replace(event.find("change@"), 7, "G_EVENTTYPE=changedevice;G_DEVICE=");

    bool matched = false;
    for (std::list<eventhandler>::iterator iter = eventsubscriptions.begin(); iter != eventsubscriptions.end(); ++iter)
    {
        if (iter->match.search(event))
        {
            pcrepp::Pcre splitregex(";", "g");
            _notify->lock();
            BashAction * action = new BashAction("run-function", iter->filename, iter->function, splitregex.split(event));
            _notify->setaction(action);
            _notify->signal();
            _notify->wait();
            _notify->unlock();

            matched = true;
            if (UEventConfiguration->get_option("log_matched_events") == "yes")
            {
                std::string retval("matched netlink-uevent: " + event);
//                std::cout << retval << std::endl;
            }
        }
    }
    if (!matched && UEventConfiguration->get_option("log_unmatched_events") == "yes")
    {
        std::string retval("unmatched netlink-uevent: " + event);
//        std::cout << retval << std::endl;
    }
}

void * NetlinkUevent::GetEvent()
{
    fd_set read_descriptors;

    FD_ZERO(&read_descriptors);
    FD_SET(netlinksocket, &read_descriptors);

    if (select(netlinksocket + 1, &read_descriptors, NULL, NULL, NULL) == -1)
    {
        std::perror("select");
        pthread_exit((void *) -1);
    }

    if (FD_ISSET(netlinksocket, &read_descriptors))
    {
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

        recvmsg(netlinksocket, &msg, 0);
        struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
        if (cmsg == NULL || cmsg->cmsg_type != SCM_CREDENTIALS)
            return NULL;
        struct ucred * cred = reinterpret_cast<ucred *> CMSG_DATA(cmsg);
        if (cred->uid != 0)
            return NULL;

        std::stringstream event_strings(netlink_buffer);
        std::string event_envvars;
        std::getline(event_strings, event_envvars);

        ProcessEvent(event_envvars);
    }
    return NULL;
}