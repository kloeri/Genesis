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

#ifndef SRC_GUARD_ACTIONS_BASH_ACTION_HH
#define SRC_GUARD_ACTIONS_BASH_ACTION_HH 1

#include <actions/action.hh>
#include <string>
#include <vector>

class BashAction : public Action {
private:
  std::string _command;
  std::string _script;
  std::string _function;
  std::vector<std::string> _env;
  std::string _result;

public:
  // Construct Action object
  BashAction(std::string command, std::string script, std::string function,
             std::vector<std::string> env)
      : _command(command), _script(script), _function(function), _env(env) {}
  BashAction(std::string command, std::string script)
      : _command(command), _script(script) {}

  // Destruct Action object
  virtual ~BashAction();

  // Perform the action
  void Execute();

  // Return the result of the action
  std::string GetResult();

  // Return information on the action (filename, function, ..) for logging
  // purposes
  std::string Identity();
};

#endif
