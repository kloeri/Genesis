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

#include "util/tokenise.hh"

using namespace genesis::util;

std::vector<std::string>
genesis::util::tokenise(const std::string & string, const std::string & delimiter)
{
    std::vector<std::string> tokens;
    std::string::size_type start, end;

    start = string.find_first_not_of(delimiter, 0);
    end = string.find_first_of(delimiter, start);

    while (end != std::string::npos || start != std::string::npos)
    {
        tokens.push_back(string.substr(start, end - start));
        start = string.find_first_not_of(delimiter, end);
        end = string.find_first_of(delimiter, start);
    }

    return tokens;
}

