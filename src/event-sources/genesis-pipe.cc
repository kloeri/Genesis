/* vim: set sw=2 sts=2 et foldmethod=syntax : */
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

#include "event-sources/genesis-pipe.hh"

#include "actions/genesis-action.hh"
#include "util/log.hh"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cerrno>
#include <cstring>
#include <unistd.h>

#define GENESIS_PIPE ("Genesis-Pipe")

using namespace genesis::events;
using namespace genesis::logging;

const std::string GenesisPipe::Name = "/dev/genesis";

GenesisPipe::GenesisPipe(void) {
  if (::mkfifo(GenesisPipe::Name.c_str(), 0666) != 0) {
    Log::get_instance().log(ERR, GENESIS_PIPE,
                            "Unable to create pipe: " +
                                std::string(::strerror(errno)));
    return; // XXX Should this throw an exception?
  }

  if (::chmod(GenesisPipe::Name.c_str(), 0666) != 0) {
    Log::get_instance().log(ERR, GENESIS_PIPE,
                            "Unable to chmod pipe: " +
                                std::string(::strerror(errno)));
    return; // XXX Should this throw an exception?
  }

  if ((_fd = ::open(GenesisPipe::Name.c_str(), O_RDWR)) < 0) {
    Log::get_instance().log(ERR, GENESIS_PIPE,
                            "Unable to open pipe: " +
                                std::string(::strerror(errno)));
    return; // XXX Should this throw an exception?
  }
}

GenesisPipe::~GenesisPipe(void) {
  if (::close(_fd) != 0) {
    Log::get_instance().log(ERR, GENESIS_PIPE,
                            "Unable to close pipe: " +
                                std::string(::strerror(errno)));
  }

  if (::unlink(GenesisPipe::Name.c_str()) != 0) {
    Log::get_instance().log(ERR, GENESIS_PIPE,
                            "Unable to remove pipe: " +
                                std::string(::strerror(errno)));
  }
}

std::unique_ptr<Action> GenesisPipe::process_event(void) {
  char buffer[256];
  ssize_t length;

  length = ::read(_fd, &buffer, sizeof(buffer));
  // Make sure to terminate the char * string
  buffer[length] = '\0';
  // And kill any newline..
  if (buffer[length - 1] == '\n')
    buffer[length - 1] = '\0';

  if (strcmp(buffer, "exit") == 0)
    return std::unique_ptr<Action>(new GenesisAction("exit"));

  // return std::unique_ptr<Action>(NULL);
  return nullptr;
}

void GenesisPipe::event_processed(const std::string &event) {}
