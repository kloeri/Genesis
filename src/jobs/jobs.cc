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

#include <fstream>
#include <iostream>
#include <regex>

#include "jobs/jobs.hh"

namespace {
  bool non_empty(std::string name, std::string match, std::string action) {
    if (name == "" || match == "" || action == "")
      return false;
    return true;
  }
}

JobsConfiguration::JobsConfiguration(std::string conffile) {
  Construct(conffile);
}

void JobsConfiguration::Construct(std::string conffile) {
  std::ifstream config(conffile.c_str());

  std::string jobname;
  std::string matching;
  std::string action;

  std::regex regex_jobname("\\[[a-zA-Z][a-zA-Z0-9]*\\]");
  std::regex regex_comment("#.*");
  std::regex regex_matching("match=[a-zA-Z]+:.+");
  std::regex regex_action("action=(bash|python):.+");

  if (config) {
    while (!config.eof()) {
      std::string line;
      std::getline(config, line);

      if (non_empty(jobname, matching, action)) {
        jobs.push_back(job(jobname, matching, action));

        jobname = "";
        matching = "";
        action = "";
      }

      // Remove comments
      if (std::regex_match(line.begin(), line.end(), regex_comment)) {
        continue;
      }

      if (std::regex_match(line.begin(), line.end(), regex_jobname)) {
        jobname = line.substr(1, line.size() - 2);
        matching = "";
        continue;
      }
      if (std::regex_match(line.begin(), line.end(), regex_matching)) {
        matching = line.substr(6, line.size() - 6);
        continue;
      }
      if (std::regex_match(line.begin(), line.end(), regex_action)) {
        action = line.substr(7, line.size() - 7);
        continue;
      }
    }
  }
}

std::string JobsConfiguration::get_option(const std::string key) const {
  return nullptr;
}

void JobsConfiguration::show_all() {
  for (auto j : jobs) {
    std::cout << "job: " << j.name << ", " << j.match << ", " << j.action << std::endl;
  }
}
