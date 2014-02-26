/*
 * cg_textfile_parser.hpp
 *
 *  Created on: Feb 26, 2014
 *      Author: luc.martel
 */

#ifndef CG_TEXTFILE_PARSER_HPP_
#define CG_TEXTFILE_PARSER_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class cg_text_file {
public:
   cg_text_file() : mInitialized(false), mFilename("") {}
   cg_text_file(std::string filename) :  mInitialized(false), mFilename(filename) {}

   ~cg_text_file() {
      reset();
   }

   void reset() {
      if (mInitialized) {
         mInitialized = false;
         mInFileStream.close();
      }
   }

   bool open() {
      reset();
      mInFileStream.open(mFilename.c_str());

      if (mInFileStream.good()) {
         mInitialized = true;
      }
      else {
         mInitialized = true;
      }

      return mInitialized;
   }

   bool next_line(std::string &line) {
      bool rval = true;
      if (mInitialized) {
         if (!(std::getline(mInFileStream, line))) {
            rval = false;
         }
      }
      else {
         rval = false;
      }
      return rval;
   }

private:
   bool          mInitialized;

   std::string   mFilename;
   std::ifstream mInFileStream;
};


#endif /* CG_TEXTFILE_PARSER_HPP_ */
