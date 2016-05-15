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

#ifndef SRC_GENESIS_HANDLER_GUARD_JOBS_HH
#define SRC_GENESIS_HANDLER_GUARD_JOBS_HH 1

#include <map>
#include <string>

class JobsConfiguration {
private:
  std::string section;
  mutable std::map<std::string, std::string> options;

public:
  JobsConfiguration(std::string conffile);
  void Construct(std::string conffile);
  std::string get_option(const std::string key) const;
};

#endif
