/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <iostream>
#include <string>
#include <bash.hh>
#include "bash-action.hh"

void BashAction::Execute()
{
    if (_command == "get-metadata")
    {
        _result = GetMetadata(_script);
    }
    if (_command == "run-function")
    {
        RunBashFunction(_script, _function, _env);
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

