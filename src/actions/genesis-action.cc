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

#include <cstdlib>
#include <iostream>
#include <string>
#include "genesis-action.hh"

#include "util/log.hh"
#include "genesis-handler/event-listener.hh"

#define GENESIS_ACTION          ("Genesis-Action")

using namespace genesis;
using namespace genesis::logging;

void GenesisAction::Execute()
{
    if (command == "exit")
    {
        Log::get_instance().log(INFO, GENESIS_ACTION, "Genesis received exit command.");
        EventListener::get_instance().terminate(true);
    }
}

std::string GenesisAction::GetResult()
{
    return "";
}

std::string GenesisAction::Identity()
{
    return command;
}

