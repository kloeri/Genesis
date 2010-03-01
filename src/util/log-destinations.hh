/* vim: set sw=4 sts=4 et foldmethod=syntax : */
/*
 * Copyright (c) 2010 Saleem Abdulrasool
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

#ifndef SRC_UTIL_LOG_DESTINATIONS_HH
#define SRC_UTIL_LOG_DESTINATIONS_HH

#include <syslog.h>

#include <fstream>
#include <sstream>
#include <streambuf>

namespace genesis
{
    namespace logging
    {
        namespace internals
        {
            struct null_streambuf
                : public std::streambuf
            {
                int_type
                overflow(int_type c)
                {
                    return std::char_traits<char>::not_eof(c);
                }
            };

            struct null_streambuf_init
            {
                null_streambuf _buffer;

                std::streambuf *
                rdbuf(void)
                {
                    return &_buffer;
                }
            };

            struct fstream_init
            {
                std::ofstream _stream;

                fstream_init(const std::string & path, std::ios_base::openmode mode = std::ios_base::out)
                    : _stream(path, mode)
                {
                }

                std::streambuf *
                rdbuf(void)
                {
                    return _stream.rdbuf();
                }
            };

            struct syslog_streambuf
                : public std::streambuf
            {
                std::ostringstream _buffer;
                typedef std::char_traits<char> traits;

                int_type
                overflow(int_type c)
                {
                    if (! traits::eq_int_type(c, traits::eof()))
                        _buffer << traits::to_char_type(c);
                    return traits::not_eof(c);
                }

                int
                sync(void)
                {
                    ::syslog(0, _buffer.str().c_str());
                    _buffer.str("");
                    return 0;
                }
            };

            struct syslog_streambuf_init
            {
                syslog_streambuf _buffer;

                std::streambuf *
                rdbuf(void)
                {
                    return &_buffer;
                }
            };
        }

        class null_stream
            : private virtual internals::null_streambuf_init,
              public std::ostream
        {
            private:
                typedef null_streambuf_init buffer;

            public:
                null_stream(void)
                    : std::ostream(buffer::rdbuf())
                {
                }
        };

        class console_stream
            : public std::ostream
        {
            public:
                console_stream(void)
                    : std::ostream(std::clog.rdbuf())
                {
                }
        };

        class file_stream
            : private virtual internals::fstream_init,
              public std::ostream
        {
            private:
                typedef fstream_init buffer;

            public:
                file_stream(const std::string & path, std::ios_base::openmode mode = std::ios_base::out)
                    : fstream_init(path, mode), std::ostream(buffer::rdbuf())
                {
                }
        };

        class syslog_stream
            : private virtual internals::syslog_streambuf_init,
              public std::ostream
        {
            private:
                typedef syslog_streambuf_init buffer;

            public:
                syslog_stream(void)
                    : std::ostream(buffer::rdbuf())
                {
                }
        };
    }
}

#endif

