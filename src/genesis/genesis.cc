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

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>

#include "genesis/genesis_command_line.hh"

using namespace genesis;

namespace {
struct display_help {};

int terminate_genesis_handler(void) {
  std::ofstream genesis_pipe("/dev/genesis");
  if (!genesis_pipe) {
    std::cerr << "Unable to open genesis pipe.  Is genesis-handler running?"
              << std::endl;
    return EXIT_FAILURE;
  }

  genesis_pipe << "exit";

  return EXIT_SUCCESS;
}
}

int main(int argc, char *argv[]) {
  genesis_command_line command_line;

  try {
    command_line.parse(argc, argv);

    if (command_line.help.specified())
      throw display_help();

    if (command_line.terminate.specified())
      return terminate_genesis_handler();
  } catch (const display_help &) {
    std::cout << "usage: " << argv[0] << " [options]" << std::endl;
    std::cout << command_line;
    return EXIT_SUCCESS;
  } catch (const bad_argument &ex) {
    std::cerr << "Unknown argument: " << ex.argument << std::endl;
    std::cerr << "See --help for usage information" << std::endl;
    return EXIT_FAILURE;
  } catch (const std::exception &ex) {
    std::cerr << "Unhandled exception:" << std::endl << ex.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unhandled exception:" << std::endl
              << "  An unknown exception occurred." << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
