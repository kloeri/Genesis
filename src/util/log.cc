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

#include "util/log.hh"

using namespace genesis::logging;

Log::Log(std::unique_ptr<std::ostream> dest)
    : _dest(std::move(dest)), _minimum_log_level(ERR)
{
}

void
Log::set_minimum_log_level(const LogLevel level)
{
    _minimum_log_level = level;
}

void
Log::log(const LogLevel level, const std::string & tag, const std::string & message)
{
    (*_dest) << level << tag << ": " << message << std::endl;
}

std::ostream &
genesis::logging::operator<<(std::ostream & os, const LogLevel level)
{
    switch (level)
    {
        case DEBUG:
            return os << "debug: ";
        case INFO:
            return os << "info: ";
        case NOTICE:
            return os << "notice: ";
        case WARN:
            return os << "warn: ";
        case ERR:
            return os << "error: ";
        case CRIT:
            return os << "critical: ";
    }

    return os;
}

