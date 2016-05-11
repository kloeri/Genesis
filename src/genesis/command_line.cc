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

#include "command_line.hh"
#include "argument.hh"

#include <sstream>

using namespace genesis;

command_line::command_line(void) {}

command_line::~command_line(void) {}

void command_line::add_argument(argument *argument) {
  _arguments.push_back(argument);
  _long_names[argument->long_name] = argument;
  if (argument->short_name != '0')
    _short_names[argument->short_name] = argument;
}

const argument *command_line::get_argument(const std::string &long_name) const {
  std::map<std::string, argument *>::const_iterator argument(
      _long_names.find(long_name));

  if (argument == _long_names.end())
    return NULL;
  else
    return argument->second;
}

void command_line::parse(const int argc, const char *const *const argv) {
  std::list<std::string> arguments(argv + 1, &argv[argc]);
  std::list<std::string>::iterator argument;

  for (argument = arguments.begin(); argument != arguments.end(); ++argument) {
    std::string parameter(*argument);

    if (parameter == "--") {
      argument++;
      break;
    } else if (parameter.compare(0, 2, "--") == 0) {
      parameter.erase(0, 2);
      std::map<std::string, genesis::argument *>::const_iterator it(
          _long_names.find(parameter));
      if (it == _long_names.end())
        throw bad_argument("--" + parameter);
      it->second->specified(true);
    } else if (parameter.compare(0, 1, "-") == 0) {
      parameter.erase(0, 1);
      for (std::string::const_iterator p(parameter.begin());
           p != parameter.end(); p++) {
        std::map<char, genesis::argument *>::const_iterator it(
            _short_names.find(*p));
        if (it == _short_names.end())
          throw bad_argument("-" + std::string(1, *p));
        it->second->specified(true);
      }
    } else {
      _parameters.push_back(parameter);
    }
  }

  _parameters.insert(_parameters.end(), argument, arguments.end());
}

const std::list<std::string> &command_line::parameters(void) const {
  return _parameters;
}

std::ostream &genesis::operator<<(std::ostream &os,
                                  const command_line &command_line) {
  std::list<genesis::argument *>::const_iterator argument;

  for (argument = command_line._arguments.begin();
       argument != command_line._arguments.end(); ++argument) {
    std::ostringstream display;

    display << "  --" << (*argument)->long_name;
    if ((*argument)->short_name)
      display << ", -" << (*argument)->short_name;

    if (display.str().length() < 20)
      display << std::string(20 - display.str().length(), ' ');

    display << (*argument)->description;

    os << display.str() << std::endl;
  }

  return os;
}
