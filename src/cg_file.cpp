/*
 * cg_file.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: lmartel
 */

// Kludge to work around a GCC 4.8.1 bug around off64_t when std=c++0x is defined
#undef __STRICT_ANSI__

#include <sys/types.h>
#include <dirent.h>

#include <iostream>
#include <string.h>
#include "cg_file.hpp"


cg_file::cg_file() :
   mType(cg_file::FILE),
   mName(""),
   mPath("") {}

cg_file::cg_file(cg_file::TYPE t, std::string n, std::string p) :
   mType(t),
   mName(n),
   mPath(p) {}

cg_file::~cg_file() {}

std::ostream &operator<<(std::ostream &os, cg_file &file) {
   os << file.mPath << "/" << file.mName;

   return os;
}

cg_file::TYPE cg_file::get_type() {
   return mType;
}

std::string cg_file::get_name() {
   return mName;
}

std::string cg_file::get_path() {
   return mPath;
}


cg_file_mgr::cg_file_mgr() {}
cg_file_mgr::~cg_file_mgr() {}

bool
cg_file_mgr::get_file_list(std::string path, std::list<std::string> &s) {
   bool rval = true;

   DIR *dir;
   struct dirent *entry;

   if (!(dir = opendir(path.c_str()))) {
       return rval;
   }
   if (!(entry = readdir(dir))) {
       closedir(dir);
       return rval;
   }

   do {
      std::string temp_path;
      temp_path += path;
      temp_path += "/";
      temp_path += entry->d_name;

      s.push_back(temp_path);

      // If it's a directory
      if (entry->d_type == DT_DIR) {
         if (!(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)) {
            get_file_list(temp_path, s);
         }
      }
   } while ((entry = readdir(dir)));

   closedir(dir);

   return rval;
}

bool cg_file_mgr::get_file_object_list(std::string path, std::list<cg_file> &s) {
   bool rval = true;

   get_filtered_file_object_list(path, s, *[](cg_file &file) -> bool {return true;});

   return rval;
}

bool cg_file_mgr::get_filtered_file_object_list(std::string path, std::list<cg_file> &s, bool (*f)(cg_file &)) {
   bool rval = true;

   DIR *dir;
   struct dirent *entry;

   if (!(dir = opendir(path.c_str()))) {
       return rval;
   }
   if (!(entry = readdir(dir))) {
       closedir(dir);
       return rval;
   }

   do {
      std::string temp_path;
      temp_path += path;
      temp_path += "/";
      temp_path += entry->d_name;

      cg_file file(((entry->d_type == DT_DIR) ? cg_file::DIR : cg_file::FILE), std::string(entry->d_name), path);

      if (f(file)) {
         s.push_back(file);
      }

      // If it's a directory
      if (entry->d_type == DT_DIR) {
         if (!(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)) {
            get_filtered_file_object_list(temp_path, s, f);
         }
      }
   } while ((entry = readdir(dir)));

   closedir(dir);

   return rval;
}
