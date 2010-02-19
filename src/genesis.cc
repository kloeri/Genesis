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

#include <cstring>
#include <limits.h>
#include <pcre++.h>
#include <string>
#include <sys/mount.h>

#include <pcre++.h>

#include <events/event.hh>
#include <events/netlink-uevent.hh>
#include <events/netlink-route.hh>
#include <events/genesis-fifo.hh>
#include <config.hh>
#include <utils.hh>

namespace
{
    using namespace genesis;
    const std::string GenesisVersion = "0.1alpha0";

    template<class T>
    void *create_thread(void *arg)
    {
        int* argp = static_cast<int*>(arg);
        EventManager *event = new T(*argp);

        while (true)
        {
            event->GetEvent();
        }

        return NULL;
    }
}

int main(int argc, char * argv[])
{
    WelcomeBlurb();

    // Read config file
    genesis::Configuration ModulesConfiguration(SYSCONFDIR "config", "modules");
    genesis::Configuration GenesisConfiguration(SYSCONFDIR "config", "genesis");

    // We use pipes for passing messages from event handlers to the main thread
    int pipefds[2];
    if (pipe(pipefds) == -1)
    {
        std::perror("pipe");
        exit(-1);
    }

    // Open log file
    std::ofstream Logfile;
    if (GenesisConfiguration.get_option("logging") == "file")
    {
        Logfile.open(GenesisConfiguration.get_option("logfile").c_str());
    }

    // Start netlink event handler threads
    pthread_t nl_thread[3];
    if (ModulesConfiguration.get_option("command") == "yes")
    {
        pthread_create(&nl_thread[0], NULL, create_thread<GenesisFIFO>, &pipefds[1]);
    }

    if (ModulesConfiguration.get_option("netlink-uevent") == "yes")
    {
        pthread_create(&nl_thread[1], NULL, create_thread<NetlinkUevent>, &pipefds[1]);
    }

    if (ModulesConfiguration.get_option("netlink-route") == "yes")
    {
        pthread_create(&nl_thread[2], NULL, create_thread<NetlinkRoute>, &pipefds[1]);
    }

    // Keep handling events infinitely
    while (true)
    {
        // PIPE_BUF is the largest message size that the kernel guarantees atomic writes for
        char buf[PIPE_BUF];
        ssize_t len;
        memset(&buf, 0, sizeof(buf));
        len = read(pipefds[0], buf, PIPE_BUF);

        std::stringstream events(buf);
//        memset(buf, 0, PIPE_BUF);

        while (!events.eof())
        {
            std::string eventstring;
            std::getline(events, eventstring);

            if (eventstring != "")
            {
                if (eventstring == "G_EVENTSOURCE=genesis;command=exit")
                {
                    exit(0);
                }

                if (GenesisConfiguration.get_option("logging") == "file")
                {
                    Logfile << eventstring << std::endl;
                }
                if (GenesisConfiguration.get_option("logging") == "console")
                {
                    std::cout << eventstring << std::endl;
                }
            }
        }
    }

    return 0;
}

