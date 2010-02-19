/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2009 Bryan Østergaard
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

#include <serialize.hh>
#include <iostream>
#include <list>
#include <string>

std::string serialize(std::list<std::string> items)
{
    std::string retval;

    for (std::list<std::string>::iterator iter = items.begin(); iter != items.end(); ++iter)
    {
        if (retval != "")
            retval += ";";
        retval += *iter;
    }
    return retval;
}

std::list<std::string> unserialize(std::string items)
{
    std::list<std::string> retval;

    size_t pos = 0;
    while (pos < items.size())
    {
        size_t endpos = items.find(';', pos);
        if (endpos == std::string::npos)
            endpos = items.size();
        retval.push_back(items.substr(pos, endpos - pos));
        pos = endpos + 1;
    }
    return retval;
}
