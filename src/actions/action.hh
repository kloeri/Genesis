/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef SRC_GUARD_ACTION_HH
#define SRC_GUARD_ACTION_HH 1

class Action
{
    public:
        // Execute action
        virtual void Execute() = 0;

        // Get result of executing the action
        virtual std::string GetResult() = 0;

        // Get action parameters for log purpose
        virtual std::string Identity() = 0;
};

#endif
