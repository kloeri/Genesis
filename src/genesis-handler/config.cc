/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2010 Bryan Østergaard
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

#include <iostream>
#include <fstream>
#include <pcre++.h>

#include "genesis-handler/config.hh"

Configuration::Configuration(std::string conffile, std::string sectionname)
{
    Construct(conffile, sectionname, std::map<std::string, std::string>());
}

Configuration::Configuration(std::string conffile, std::string sectionname, std::map<std::string, std::string> defconfig)
{
    Construct(conffile, sectionname, defconfig);
}

void Configuration::Construct(std::string conffile, std::string sectionname, std::map<std::string, std::string> defconfig)
{
    // Set default configuration
    for (auto  iter : defconfig)
    {
        options[iter.first] = iter.second;
    }

    std::ifstream config(conffile.c_str());

    if (config)
    {
        while (!config.eof())
        {
            std::string line;
            std::getline(config, line);

            // Remove comments
            pcrepp::Pcre regex_comments("#.*");
            line = regex_comments.replace(line, "");

            if (section == sectionname)
            {
                pcrepp::Pcre regex_module("[ \t]*\\[([a-zA-Z]+)\\][ \t]*");
                regex_module.search(line);
                if (regex_module.matches() > -1)
                {
                    section = regex_module.get_match(0);
                }

                pcrepp::Pcre regex_keyvalue("[ \t]*([a-zA-Z_-]+)[ \t]*=[ \t]*([a-zA-Z0-9/._]+)[ \t]*");
                regex_keyvalue.search(line);
                if (regex_keyvalue.matches() > -1)
                {
                    options[regex_keyvalue.get_match(0)] = regex_keyvalue.get_match(1);
                }
            }
            else if (line[0] == '[')
            {
                section = line.substr(1, line.size() - 2);
            }
        }
    }

    section = sectionname;
}

std::string Configuration::get_option(const std::string key) const
{
    return options[key];
}

