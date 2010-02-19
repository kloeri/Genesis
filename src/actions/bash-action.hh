/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef SRC_GUARD_ACTIONS_BASH_ACTION_HH
#define SRC_GUARD_ACTIONS_BASH_ACTION_HH 1

#include <string>
#include <actions/action.hh>

class BashAction : public Action
{
    private:
        std::string _command;
        std::string _script;
        std::string _function;
        std::vector<std::string> _env;
        std::string _result;

    public:
        BashAction(std::string command, std::string script, std::string function, std::vector<std::string> env)
            : _command(command), _script(script), _function(function), _env(env) {}
        BashAction(std::string command, std::string script)
            : _command(command), _script(script) {}
        void Execute();
        std::string GetResult();
        std::string Identity();
};

#endif
