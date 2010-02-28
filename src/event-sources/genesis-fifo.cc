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

#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <event-sources/genesis-fifo.hh>
#include <actions/genesis-action.hh>
#include <genesis-handler/logger.hh>

GenesisFIFO::GenesisFIFO()
{
    // Create control FIFO and make sure it has the right permissions

    int err = 0;

    err = ::mkfifo("/dev/genesis", 0666);
    if (err)
    {
        Logger::get_instance().Log(ERR, "genesis-fifo: failed to create FIFO: " + std::string(::strerror(errno)));
        return;
    }

    err = chmod("/dev/genesis", 0666);

    fd = open("/dev/genesis", O_RDWR);
}

GenesisFIFO::~GenesisFIFO()
{
    close(fd);
    unlink("/dev/genesis");
}

std::list<std::string> GenesisFIFO::get_events()
{
    return std::list<std::string>();
}

int GenesisFIFO::get_fd()
{
    return fd;
}

Action * GenesisFIFO::GetEvent()
{
    char buf[200];
    ssize_t len = read(fd, &buf, 200);
    buf[len] = '\0';
    if (strcmp(buf, "exit") == 0)
    {
        return new GenesisAction("exit");
    }
    return 0;
}

Action * GenesisFIFO::new_event(std::string event)
{
    return NULL;
}
