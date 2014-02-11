/*
 * main.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: lmartel
 */

#include <iostream>

#include "cg_basic_element.hpp"
#include "cg_file.hpp"

int
main(int argc, char *argv[]) {

   cg_file_mgr _file;

   std::list<std::string> file_list;
   _file.get_file_list(".", file_list);

   for (std::list<std::string>::iterator it = file_list.begin();
        it != file_list.end();
        ++it) {
      std::cout << *it << std::endl;
   }

   return 0;
}


