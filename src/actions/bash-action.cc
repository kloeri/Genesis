/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <iostream>
#include <string>
#include <bash.hh>
#include "bash-action.hh"

void BashAction::Execute()
{
    if (_command == "get-metadata")
    {
        _result = genesis::GetMetadata(_script);
    }
    if (_command == "run-function")
    {
        genesis::RunBashFunction(_script, _function, _env);
    }
    if (_command == "print-filename")
    {
        std::cout << "filename: " << _script << std::endl;
    }
}

std::string BashAction::GetResult()
{
    return _result;
}

std::string BashAction::Identity()
{
    return _command + ": " + _script + "->" + _function;
}

