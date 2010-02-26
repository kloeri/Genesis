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

#ifndef SRC_GUARD_ACTIONS_GENESIS_ACTION_HH
#define SRC_GUARD_ACTIONS_GENESIS_ACTION_HH 1

#include <string>
#include <actions/action.hh>

class GenesisAction : public Action
{
    private:
        std::string command;

    public:
        // Construct Action object
        GenesisAction(std::string cmd) : command(cmd) {}

        // Perform the action
        void Execute();

        // Get result from the action
        std::string GetResult();

        // Data about the action for logging purposes
        std::string Identity();
};

#endif
