/* vim: set sw=2 sts=2 et foldmethod=syntax : */
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

#ifndef __SRC_GENESIS_GENESIS_COMMAND_LINE_HH__
#define __SRC_GENESIS_GENESIS_COMMAND_LINE_HH__

#include "genesis/argument.hh"
#include "genesis/command_line.hh"

namespace genesis {
class genesis_command_line : public command_line {
public:
  genesis::switched_argument help;
  genesis::switched_argument terminate;

  genesis_command_line(void);
  virtual ~genesis_command_line(void);
};
};

#endif
