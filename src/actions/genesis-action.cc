/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <cstdlib>
#include <iostream>
#include <string>
#include "genesis-action.hh"

void GenesisAction::Execute()
{
    if (command == "exit")
    {
        std::cout << "Genesis received exit command." << std::endl;
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

