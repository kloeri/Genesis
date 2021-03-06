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

#include <iostream>
#include <string>

#include "actions/bash-action.hh"
#include "genesis-handler/bash.hh"

void BashAction::Execute() {
  if (_command == "get-metadata") {
    _result = GetMetadata(_script);
  }
  if (_command == "run-function") {
    _result = RunBashFunction(_script, _function, _env);
  }
}

std::string BashAction::GetResult() { return _result; }

std::string BashAction::Identity() {
  return _command + ": " + _script + "->" + _function;
}

BashAction::~BashAction() {}
