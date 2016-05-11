/* vim: set sw=2 sts=2 et foldmethod=syntax : */
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

#ifndef SRC_EVENT_SOURCES_GENESIS_PIPE_HH
#define SRC_EVENT_SOURCES_GENESIS_PIPE_HH

#include <string>

#include "event-sources/event-source.hh"

namespace genesis {
namespace events {
class GenesisPipe : public EventSource {
public:
  static const std::string Name;

  GenesisPipe(void);
  ~GenesisPipe(void);

  virtual std::unique_ptr<Action> process_event(void);
  virtual void event_processed(const std::string &event);
};
}
}

#endif
