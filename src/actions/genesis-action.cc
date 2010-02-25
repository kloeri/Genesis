/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <cstdlib>
#include <iostream>
#include <string>
#include "genesis-action.hh"
#include <logger.hh>

void GenesisAction::Execute()
{
    if (command == "exit")
    {
        Logger::get_instance()->Log(INFO, "Genesis received exit command.");
        exit(0);
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

