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

#ifndef SRC_GENESIS_HANDLER_GUARD_LOGGER_HH
#define SRC_GENESIS_HANDLER_GUARD_LOGGER_HH 1

#include <ostream>
#include <sstream>

#include <genesis-handler/singleton.hh>

enum Loglevel
{
    DEBUG,
    INFO,
    NOTICE,
    WARN,
    CRIT,
    ERR
};

class LogDestination
    : public std::basic_ostream<char>
{
    protected:
        LogDestination(std::basic_streambuf<char> *rdbuf);

    public:
        virtual ~LogDestination(void);
        virtual void set_log_level(const Loglevel level) = 0;
};

class StringLogger
    : public LogDestination
{
    protected:
        StringLogger(std::basic_streambuf<char> *rdbuf);

    public:
        virtual ~StringLogger(void);
        void set_log_level(const Loglevel level);
};

class Console
    : public StringLogger
{
    public:
        Console(void);
};

class File
    : public StringLogger
{
    public:
        File(const std::string & path, std::ios_base::openmode mode = std::ios_base::out);
};

class dmesg
    : public StringLogger
{
    public:
        dmesg(void);
};

class syslog_streambuf
    : public std::basic_streambuf<char>
{
    private:
        std::ostringstream _buffer;
        typedef std::char_traits<char> traits;
        typedef std::basic_streambuf<char>::int_type int_type;
        int _log_level;

        syslog_streambuf(const syslog_streambuf &);
        syslog_streambuf & operator=(const syslog_streambuf &);

        int_type overflow(int_type c);
        int sync(void);

    public:
        syslog_streambuf(void);
        void set_log_level(const Loglevel log_level);
};

struct syslog_streambuf_init
{
    syslog_streambuf _buffer;

    std::basic_streambuf<char> *
    get(void)
    {
        return &_buffer;
    }
};

class SysLog
    : private virtual syslog_streambuf_init,
      public LogDestination
{
    private:
        typedef syslog_streambuf_init buffer;

    public:
        SysLog(void);
        ~SysLog(void);

        void set_log_level(const Loglevel level);
};

extern Console console_stream;
extern SysLog syslog_stream;

class Logger
    : public Singleton<Logger>
{
    private:
        LogDestination & _log;

        Loglevel minimum_log_level;

    public:
        Logger(LogDestination & destination = console_stream);

        void set_log_level(Loglevel minlevel);
        void Log(Loglevel level, std::string);
};

LogDestination &
operator<<(LogDestination & dest, const Loglevel level);

#endif
