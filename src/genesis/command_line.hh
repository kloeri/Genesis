/* vim: set sw=4 sts=4 et foldmethod=syntax : */
/*
 * Copyright © 2010 Saleem Abdulrasool
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

#ifndef __SRC_GENESIS_COMMAND_LINE_HH__
#define __SRC_GENESIS_COMMAND_LINE_HH__

#include <map>
#include <list>
#include <string>

namespace genesis
{
    class argument;

    struct bad_argument
    {
        const std::string argument;

        bad_argument(const std::string & argument)
            : argument(argument)
        {
        }
    };

    class command_line
    {
        friend std::ostream & operator<<(std::ostream & os, const command_line & command_line);

        private:
            std::list<argument *> _arguments;

            std::map<std::string, argument *> _long_names;
            std::map<char, argument *> _short_names;

            std::list<std::string> _parameters;

        public:
            command_line(void);
            ~command_line(void);

            void add_argument(argument *argument);
            const argument * get_argument(const std::string & long_name) const;

            void parse(const int argc, const char * const * const argv);

            const std::list<std::string> & parameters(void) const;
    };

    std::ostream & operator<<(std::ostream & os, const command_line & command_line);
}

#endif

