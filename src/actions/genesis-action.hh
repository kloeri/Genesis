/* vim: set sw=4 sts=4 et foldmethod=syntax : */

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
