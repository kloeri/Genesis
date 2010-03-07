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

#include "genesis/argument.hh"

using namespace genesis;

argument::argument(const std::string & long_name, const char short_name, const std::string & description)
    : _specified(false), description(description), long_name(long_name), short_name(short_name)
{
}

argument::~argument(void)
{
}

void
argument::specified(const bool value)
{
    _specified = value;
}

bool
argument::specified(void) const
{
    return _specified;
}

switched_argument::switched_argument(const std::string & long_name, const char short_name, const std::string & description)
    : argument(long_name, short_name, description)
{
}

switched_argument::~switched_argument(void)
{
}

