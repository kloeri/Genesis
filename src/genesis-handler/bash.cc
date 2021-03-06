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

#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "genesis-handler/bash.hh"
#include "util/log.hh"

#define BASH ("bash")

using namespace genesis::logging;

char **charptrarray(const std::vector<std::string> &env) {
  // +1 as the array needs to be NULL terminated
  char **cenv = new char *[env.size() + 1];

  for (unsigned int i = 0; i < env.size(); ++i)
    cenv[i] = const_cast<char *>(env.at(i).c_str());
  cenv[env.size()] = NULL;

  return cenv;
}

std::string GetMetadata(const std::string &file) {
  int mystdin[2];
  int mystdout[2];
  int mystderr[2];

  // Open pipes
  if (pipe(mystdin) == -1) {
    std::perror("pipe");
  }
  if (pipe(mystdout) == -1) {
    std::perror("pipe");
  }
  if (pipe(mystderr) == -1) {
    std::perror("pipe");
  }

  // Fork
  pid_t pid = fork();
  switch (pid) {
  case -1:
    std::perror("fork");
    break;
  case 0:
    // Duplicate pipes to std* file descriptors
    dup2(mystdin[0], 0);
    dup2(mystdout[1], 1);
    dup2(mystderr[1], 2);

    execl("/bin/bash", "-c", LIBEXECDIR "netlink-uevent.sh", file.c_str(),
          NULL);
    break;
  default:
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
      char buf[1024];
      ssize_t size = read(mystdout[0], buf, 1024);
      buf[size] = 0;
      return std::string(buf);
    } else {
      Log::get_instance().log(ERR, BASH, "Child exited abnormally.");
    }
  }
  return std::string();
}

std::string RunBashFunction(const std::string &file,
                            const std::string &function,
                            const std::vector<std::string> &envvars) {
  int mystdin[2];
  int mystdout[2];
  int mystderr[2];

  // Open pipes
  if (pipe2(mystdin, O_NONBLOCK) == -1) {
    std::perror("pipe");
  }
  if (pipe2(mystdout, O_NONBLOCK) == -1) {
    std::perror("pipe");
  }
  if (pipe2(mystderr, O_NONBLOCK) == -1) {
    std::perror("pipe");
  }

  // Fork
  pid_t pid = fork();
  switch (pid) {
  case -1:
    std::perror("fork");
    break;

  case 0:
    // Duplicate pipes to std* file descriptors
    dup2(mystdin[0], 0);
    dup2(mystdout[1], 1);
    dup2(mystderr[1], 2);

    char **envarray;
    envarray = charptrarray(envvars);
    execle("/bin/bash", "-c", LIBEXECDIR "bash-run.sh", file.c_str(),
           function.c_str(), NULL, envarray);
    delete[] envarray;
    break;

  default:
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status)) {
      char buf[4096] = {'\0'};
      ssize_t size = read(mystdout[0], buf, 4096);
      buf[size] = 0;
      return std::string(buf);
    } else {
      Log::get_instance().log(ERR, BASH, "Child exited abnormally.");
    }
  }
  return std::string();
}
