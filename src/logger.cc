/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <iostream>
#include "logger.hh"

Logger * Logger::get_instance()
{
    if (instance == NULL)
    {
        instance = new Logger();
    }
    return instance;
}

void Logger::set_log_level(Loglevel level)
{
    minimum_log_level = level;
}

void Logger::Log(Loglevel level, std::string message)
{
    if (level < minimum_log_level)
    {
        return;
    }

    switch (level)
    {
        case DEBUG:
            std::cout << "debug: ";
            break;
        case INFO:
            std::cout << "info: ";
            break;
        case NOTICE:
            std::cout << "notice: ";
            break;
        case WARN:
            std::cout << "warn: ";
            break;
        case ERR:
            std::cout << "error: ";
            break;
        case CRIT:
            std::cout << "critical: ";
            break;
        default:
            std::cout << "unknown loglevel";
    }
    std::cout << message << std::endl;
}

Logger * Logger::instance = NULL;

