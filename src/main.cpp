/*
 * main.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: lmartel
 */

#include <iostream>
#include <list>

#include "cg_textfile_parser.hpp"
#include "cg_tag_parser.hpp"

// #include "cg_basic_element.hpp"
// #include "cg_file.hpp"

int
main(int argc, char *argv[]) {

   // File manager test
   // ------------------------------------------------
#if 0
   // We'll need a file manager
   cg_file_mgr _file;

   std::list<cg_file> file_list;
   std::list<cg_file> file_list_m3u8;
   std::list<cg_file> file_list_ts;

   // Get all files
   //_file.get_file_object_list("HLS", file_list);

   // Get list of m3u8
   _file.get_filtered_file_object_list("HLS", file_list_m3u8, *[](cg_file &x) -> bool {
      bool rval = false;
      const std::string &name = x.get_name();
      if ((name.size() > 4) && (name.compare(name.size()-4, 4, "m3u8") == 0)) {
         rval = true;
      }
      return rval;
   });

   // Get list of ts
   _file.get_filtered_file_object_list("HLS", file_list_ts, *[](cg_file &x) -> bool {
      bool rval = false;
      const std::string &name = x.get_name();
      if ((name.size() > 2) && (name.compare(name.size()-2, 2, "ts") == 0)) {
         rval = true;
      }
      return rval;
   });

   // M3U8
   std::cout << "Playlists - M3U8" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   for (std::list<cg_file>::iterator it = file_list_m3u8.begin();
        it != file_list_m3u8.end();
        ++it) {
      std::cout << *it << std::endl;
   }
   std::cout << std::endl;

   // TS
   std::cout << "TS Segments" << std::endl;
   std::cout << "-------------------------------------" << std::endl;
   for (std::list<cg_file>::iterator it = file_list_ts.begin();
        it != file_list_ts.end();
        ++it) {
      std::cout << *it << std::endl;
   }
   std::cout << std::endl;
#endif


   // Text file parser test
#if 1
   //cg_text_file       _textFile("stream4.m3u8");
   cg_text_file       _textFile("BigBuckBunny_HLSProtected.m3u8");
   cg_tag_line_parser _tagLineParser;

   if (_textFile.open()) {
      std::string s;
      while (_textFile.next_line(s)) {

         std::cout << "****************************************************" << std::endl;
         std::cout << s << std::endl << std::endl;

         cg_tag_line tag_line = _tagLineParser.lex(s);

         if (tag_line.isOK()) {
            std::cout << tag_line;
         }
      }
   }

#endif


   return 0;
}


