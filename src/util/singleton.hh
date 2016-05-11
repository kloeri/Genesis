/* vim: set et fdm=syntax sts=2 sw=2 : */
/*
 * Copyright © 2000 Scott Bilas
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

#ifndef SRC_UTIL_SINGLETON_HH
#define SRC_UTIL_SINGLETON_HH

#include <cassert>

template <typename type_> class Singleton {
private:
  static type_ *_instance;

public:
  Singleton(void) {
    assert(!_instance);
    _instance = static_cast<type_ *>(this);
  }

  virtual ~Singleton(void) {
    assert(_instance);
    _instance = NULL;
  }

  static type_ &get_instance(void) {
    assert(_instance);
    return *_instance;
  }
};

template <typename type_> type_ *Singleton<type_>::_instance = NULL;

#endif
