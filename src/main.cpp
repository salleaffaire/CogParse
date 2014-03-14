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

#include "cg_state_machine.hpp"
#include "cg_state_tests.hpp"

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

   // Test state machine
#if 1
   cg_state_machine sm(3);
   sm.set_state(new cg_st_tts_start, 0);
   sm.set_state(new cg_st_tts_query, 1);
   sm.set_state(new cg_st_tts_end,   2);

   sm.run();
#endif


   // Text file parser test
#if 0
   //cg_text_file       _textFile("stream4.m3u8");
   cg_text_file       _textFile("BigBuckBunny_HLSProtected.m3u8");
   cg_tag_line_parser _tagLineParser;

   if (_textFile.open()) {
      std::string s;
      while (_textFile.next_line(s)) {

         std::cout << "********************************************************************************" << std::endl;
         std::cout << s << std::endl << std::endl;

         cg_tag_line tag_line = _tagLineParser.lex(s);

         if (tag_line.isOK()) {
            std::cout << "Parsed SUCCESS !! " << std::endl;
            std::cout << "--------------------------------" << std::endl;
            std::cout << tag_line;
         }
         else {
            std::cout << "ERROR !! " << std::endl;
         }
      }
   }

#endif


   return 0;
}


