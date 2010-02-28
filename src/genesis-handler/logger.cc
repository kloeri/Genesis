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

