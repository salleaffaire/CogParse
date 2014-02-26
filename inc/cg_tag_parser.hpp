/*
 * cg_tag_parser.hpp
 *
 *  Created on: Feb 26, 2014
 *      Author: luc.martel
 */

#ifndef CG_TAG_PARSER_HPP_
#define CG_TAG_PARSER_HPP_

#include <string>
#include <map>
#include <list>

// Default is for HLS
// ----------------------------------------------
#define CG_DEFAULT_TAG_START '#'
#define CG_DEFAULT_TAG_END   ':'
#define CG_DEFAULT_ATTRIBUTE_DELIMITER ','
#define CG_DEFAULT_KEY_VALUE_SEPERATOR '='
// ----------------------------------------------

class cg_tag_line_parser;

class cg_tag_line {
public:
   cg_tag_line() : mState(true) {}
   ~cg_tag_line() {}

private:
   std::string mTagName;
   std::map<std::string, std::string> mAttributes;

   //
   bool mState;

   friend class cg_tag_line_parser;
};

class cg_tag_line_parser {
public:
   enum CG_TAG_LINE_PARSE_STATE {
      CG_TAG_LINE_PARSE_STATE_DECODING_TAG_NAME        = 1,
      CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_KEY   = 2,
      CG_TAG_LINE_PARSE_STATE_DECODING_ATTRUBUTE_VALUE = 3,
   };

   cg_tag_line_parser() : mTagStart(CG_DEFAULT_TAG_START),
                          mTagEnd(CG_DEFAULT_TAG_END),
                          mAttributeDelimiter(CG_DEFAULT_ATTRIBUTE_DELIMITER),
                          mKeyValueSeperator(CG_DEFAULT_KEY_VALUE_SEPERATOR),
                          mState(CG_TAG_LINE_PARSE_STATE_DECODING_TAG_NAME) {}

   virtual ~cg_tag_line_parser() {}

   cg_tag_line parse(std::string &s) {
      cg_tag_line rval;

      std::string::iterator it = s.begin();

      if (*it != mTagStart) {
         rval.mState = false;
      }
      else {
         // Decode the startcode
         for (;it != s.end();++it) {

            std::cout << *it;
         }
      }
      std::cout << std::endl;

      return rval;
   }

private:
   char mTagStart;
   char mTagEnd;
   char mAttributeDelimiter;
   char mKeyValueSeperator;

   cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE mState;
};


#endif /* CG_TAG_PARSER_HPP_ */
