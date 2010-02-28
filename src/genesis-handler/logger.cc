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

#include <fstream>
#include <iostream>

#include <syslog.h>

#include "logger.hh"

LogDestination::LogDestination(std::basic_streambuf<char> *rdbuf)
    : std::basic_ostream<char>(rdbuf)
{
}

LogDestination::~LogDestination(void)
{
}

StringLogger::StringLogger(std::basic_streambuf<char> *rdbuf)
    : LogDestination(rdbuf)
{
}

StringLogger::~StringLogger(void)
{
}

void
StringLogger::set_log_level(const Loglevel level)
{
    std::basic_ostream<char> & os(*static_cast<std::basic_ostream<char> *>(this));

    switch (level)
    {
        case DEBUG:
            os << "debug: ";
            break;
        case INFO:
            os << "info: ";
            break;
        case NOTICE:
            os << "notice: ";
            break;
        case WARN:
            os << "warn: ";
            break;
        case ERR:
            os << "error: ";
            break;
        case CRIT:
            os << "critical: ";
            break;
    }
}

Console::Console(void)
    : StringLogger(std::clog.rdbuf())
{
}

File::File(const std::string & path, std::ios_base::openmode mode)
    : StringLogger(std::ofstream(path.c_str(), mode).rdbuf())
{
}

dmesg::dmesg(void)
    : StringLogger(std::clog.rdbuf())
{
}

syslog_streambuf::syslog_streambuf(void)
{
}

syslog_streambuf::int_type
syslog_streambuf::overflow(int_type c)
{
    if (! traits::eq_int_type(c, traits::eof()))
        _buffer << traits::to_char_type(c);
    return traits::not_eof(c);
}

int
syslog_streambuf::sync(void)
{
    std::string temp(_buffer.str());

    if (! temp.empty())
    {
        ::syslog(_log_level, temp.c_str());
        _buffer.str("");
    }

    return 0;
}

void
syslog_streambuf::set_log_level(const Loglevel level)
{
    // sync before setting the log leve to ensure that the current message is
    // logged at the correct level
    sync();

    switch (level)
    {
        case DEBUG:
            _log_level = LOG_DEBUG;
            break;
        case INFO:
            _log_level = LOG_INFO;
            break;
        case NOTICE:
            _log_level = LOG_NOTICE;
            break;
        case WARN:
            _log_level = LOG_WARNING;
            break;
        case ERR:
            _log_level = LOG_ERR;
            break;
        case CRIT:
            _log_level = LOG_CRIT;
            break;
    }
}

SysLog::SysLog(void)
    : LogDestination(buffer::get())
{
}

void
SysLog::set_log_level(const Loglevel level)
{
    _buffer.set_log_level(level);
}

SysLog::~SysLog(void)
{
}

Logger::Logger(LogDestination & destination)
    : _log(destination)
{
}

void Logger::set_log_level(Loglevel level)
{
    minimum_log_level = level;
}

void Logger::Log(Loglevel level, std::string message)
{
    if (level >= minimum_log_level)
    {
        _log << level << message << std::endl;
    }
}

LogDestination &
operator<<(LogDestination & dest, const Loglevel level)
{
    dest.set_log_level(level);
    return dest;
}

Console console_stream;
SysLog syslog_stream;

