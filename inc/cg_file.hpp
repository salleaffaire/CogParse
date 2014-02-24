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
#include <iostream>


class cg_file {
public:
   enum TYPE {
      FILE,
      DIR,
      ERR
   };

   cg_file();
   cg_file(cg_file::TYPE t, std::string n, std::string path);
   ~cg_file();

   friend std::ostream &operator<<(std::ostream &os, cg_file &file);

   cg_file::TYPE get_type();
   std::string   get_name();
   std::string   get_path();
private:
   cg_file::TYPE mType;
   std::string   mName;
   std::string   mPath;
};

template <char *ext, int size>
bool with_extension(cg_file &x) {
   bool rval = false;
   const std::string &name = x.get_name();

   if ((name.size() > size) && (name.compare(name.size()-size, size, ext) == 0)) {
      rval = true;
   }
   return rval;
}


class cg_file_mgr {
public:
   cg_file_mgr();
   ~cg_file_mgr();

   static bool get_file_list(std::string path, std::list<std::string> &s);
   static bool get_file_object_list(std::string path, std::list<cg_file> &s);
   static bool get_filtered_file_object_list(std::string path, std::list<cg_file> &s, bool (*f)(cg_file &));

private:


};



#endif /* CG_FILE_HPP_ */
