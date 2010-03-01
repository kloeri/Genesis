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

#ifndef SRC_UTIL_LOG_HH
#define SRC_UTIL_LOG_HH

#include <memory>
#include <ostream>
#include <iostream>

#include "util/singleton.hh"
#include "util/log-destinations.hh"

namespace genesis
{
    namespace logging
    {
        enum LogLevel
        {
            DEBUG,
            INFO,
            NOTICE,
            WARN,
            CRIT,
            ERR,
        };

        class Log
            : public Singleton<Log>
        {
            private:
                std::unique_ptr<std::ostream> _dest;
                LogLevel _minimum_log_level;

            public:
                Log(std::unique_ptr<std::ostream> dest = std::unique_ptr<std::ostream>(new console_stream));

                void set_minimum_log_level(const LogLevel level);
                void log(const LogLevel level, const std::string & tag, const std::string & message);
        };

        std::ostream &
        operator<<(std::ostream & os, const LogLevel level);
    }
}

#endif

