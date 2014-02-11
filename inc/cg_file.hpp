/*
 * cg_file.hpp
 *
 *  Created on: Feb 8, 2014
 *      Author: lmartel
 */

#ifndef CG_FILE_HPP_
#define CG_FILE_HPP_

#include <string>
#include <list>


class cg_file {
public:
   enum TYPE {
      FILE,
      DIR,
      ERR
   };

   cg_file();
   ~cg_file();

private:
   cg_file::TYPE mType;
   std::string mName;
   std::string mPath;
};

class cg_file_mgr {
public:
   cg_file_mgr();
   ~cg_file_mgr();

   static bool get_file_list(std::string path, std::list<std::string> &s);
   static bool get_file_object_list(std::string path, std::list<cg_file> &s);
   static bool get_filtered_file_object_list(std::string path, std::list<cg_file> &s, bool (*f)(cg_file));
   static std::string get_path_to(std::string file);

private:


};



#endif /* CG_FILE_HPP_ */
