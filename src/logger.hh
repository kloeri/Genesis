/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef SRC_GUARD_LOGGER_HH
#define SRC_GUARD_LOGGER_HH 1

enum Loglevel
{
    DEBUG,
    INFO,
    NOTICE,
    WARN,
    CRIT,
    ERR
};

class Logger
{
    private:
        static Logger * instance;
        Loglevel minimum_log_level;

    public:
        static Logger * get_instance();
        void set_log_level(Loglevel minlevel);
        void Log(Loglevel level, std::string);
};

#endif
