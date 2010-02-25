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

#ifndef SRC_GUARD_BASH_HH
#define SRC_GUARD_BASH_HH 1

#include <string>
#include <vector>

char **charptrarray(const std::vector<std::string> & env);

// Sources file and returns SUBSCRIPTION_* variables from it
std::string GetMetadata(const std::string & file);

// Runs specified function with predetermined environment
void RunBashFunction(const std::string & file, const std::string & function, const std::vector<std::string> & envvars);

#endif
