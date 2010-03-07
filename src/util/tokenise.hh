/* vim: set sw=4 sts=4 et foldmethod=syntax : */
/*
 * Copyright © 2010 Bryan Østergaard
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

#ifndef SRC_UTIL_TOKENISE_HH
#define SRC_UTIL_TOKENISE_HH

#include <string>
#include <vector>

namespace genesis
{
    namespace util
    {
        std::vector<std::string>
        tokenise(const std::string & string, const std::string & delimiter = " ");
    }
}

#endif

