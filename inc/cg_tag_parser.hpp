/*
 * cg_tag_parser.hpp
 *
 *  Created on: Feb 26, 2014
 *      Author: luc.martel
 */

#ifndef CG_TAG_PARSER_HPP_
#define CG_TAG_PARSER_HPP_

#include <iostream>
#include <string>
#include <map>
#include <list>

#include "cg_lexer_base.hpp"

// Default is for HLS
// ----------------------------------------------
#define CG_DEFAULT_TAG_START '#'
#define CG_DEFAULT_TAG_END   ':'
#define CG_DEFAULT_ATTRIBUTE_DELIMITER ','
#define CG_DEFAULT_KEY_VALUE_SEPERATOR '='
// ----------------------------------------------

// Automaton - Not HLS :(
// ----------------------------------------------
// a := false
// b := true
//
// TAG_START
// @tag
// [TAG_END, a:=true]
// while (b=true) {
//   if (a=true) {
//     @attribute_key
//     KEY_VALUE_SEPERATOR
//     @attribute_value or @stringlitteral
//   }
//   [ATTRIBUTE_DELIMITER,b=true?b=false]
// }
//


class cg_tag_line_parser;

class cg_tag_line {
public:
   cg_tag_line() : mState(true) {}
   ~cg_tag_line() {}

   bool isOK() { return mState; }

private:
   std::string mTagName;
   std::map<std::string, std::string> mAttributes;

   //
   bool mState;

   friend class cg_tag_line_parser;
   friend std::ostream &operator<<(std::ostream &os, cg_tag_line &x);
};

std::ostream &operator<<(std::ostream &os, cg_tag_line &x) {
   os << "TAG : " << x.mTagName << std::endl;
   for (std::map<std::string, std::string>::iterator it = x.mAttributes.begin();
        it != x.mAttributes.end();
        ++it) {
      os << "  " << it->first << " = " << it->second << std::endl;
   }
   return os;
}

class cg_tag_line_parser : public cg_lexer_base {
public:
   enum CG_TAG_LINE_PARSE_STATE {
      CG_TAG_LINE_PARSE_STATE_INIT                     = 1,
      CG_TAG_LINE_PARSE_STATE_DECODING_TAG_NAME        = 2,
      CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_KEY   = 3,
      CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_VALUE = 4,
      CG_TAG_LINE_PARSE_STATE_DONE                     = 5,
      CG_TAG_LINE_PARSE_STATE_ERROR                    = 6
   };

   cg_tag_line_parser() : mTagStart(CG_DEFAULT_TAG_START),
                          mTagEnd(CG_DEFAULT_TAG_END),
                          mAttributeDelimiter(CG_DEFAULT_ATTRIBUTE_DELIMITER),
                          mKeyValueSeperator(CG_DEFAULT_KEY_VALUE_SEPERATOR),
                          mState(CG_TAG_LINE_PARSE_STATE_DECODING_TAG_NAME),

                          mIsEqual('='),
                          mIsMinus('-'),
                          mIsPlus('+'),
                          mIsSlash('/') {


      // Special attribute test list to account for the fact that our embedded base64
      // is not withing double quote
      mTestAttributeValue.push_back(&mIsInUppercaseRange);
      mTestAttributeValue.push_back(&mIsInLowercaseRange);
      mTestAttributeValue.push_back(&mIsInDecimalRange);
      mTestAttributeValue.push_back(&mIsEqual);
      mTestAttributeValue.push_back(&mIsSlash);
      mTestAttributeValue.push_back(&mIsPlus);

      // HLS TAG Name is alphanumeric values + '-' (Also inclusing lower case eventhough I shouldn't)
      mTestTagName.push_back(&mIsInUppercaseRange);
      mTestTagName.push_back(&mIsInLowercaseRange);
      mTestTagName.push_back(&mIsInDecimalRange);
      mTestTagName.push_back(&mIsMinus);

      //
      create_rule("tag",            mIsTagNameRule,        &mTestTagName,        false, lr_or);
      create_rule("attributevalue", mIsAttributeValueRule, &mTestAttributeValue, false, lr_or);
   }

   virtual ~cg_tag_line_parser() {}

   cg_tag_line lex(std::string &s) {
      cg_tag_line rval;

      std::string currentAttributeKey;
      std::string currentAttributeValue;

      mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_INIT;

      const std::string::iterator end = s.end();

      for (std::string::iterator it = s.begin();it != end;++it) {

         if (*it != ' ') {
            switch (mState) {
            case cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_INIT:
               // Found the start tag
               if (*it == mTagStart) {
                  mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_TAG_NAME;
               }
               // Error -
               else {
                  mErrorMessage = "Tag Line doesn't start with tag start code : " + mTagStart;
                  mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_ERROR;
               }
               break;
            case cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_TAG_NAME:
               if (test(*it, mIsTagNameRule)) {
                  rval.mTagName += *it;
               }
               else if (*it == mTagEnd) {
                  currentAttributeKey.clear();
                  mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_KEY;
               }
               // Error -
               else {
                  mErrorMessage += "Invalid caracter : ";
                  mErrorMessage += *it;
                  mErrorMessage += " in Tag Name";

                  mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_ERROR;
               }
               break;
            case cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_KEY:
               if (test(*it, mIsTagNameRule)) {
                  currentAttributeKey += *it;
               }
               else if (*it == mKeyValueSeperator) {
                  currentAttributeValue.clear();
                  mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_VALUE;
               }
               // Error -
               else {
                  mErrorMessage += "Invalid caracter : ";
                  mErrorMessage += *it;
                  mErrorMessage += " in Key Name";

                  mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_ERROR;
               }
               break;
            case cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_VALUE:
               if (test(*it, mIsAttributeValueRule)) {
                  currentAttributeValue += *it;
               }
               else if (*it == '"') {
                  decode_string_literal(it, end, currentAttributeValue);
               }
               else if (*it == mAttributeDelimiter) {
                  rval.mAttributes[currentAttributeKey] = currentAttributeValue;
                  currentAttributeKey.clear();
                  mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_KEY;
               }
               // Error -
               else {
                  mErrorMessage += "Invalid caracter : ";
                  mErrorMessage += *it;
                  mErrorMessage += " in Value Name";

                  mState = cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_ERROR;
               }
               break;
            case cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DONE:
            case cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_ERROR:
               break;
            }
         }
         if (mState == cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_ERROR)
            break;
      }

      if (mState == cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_KEY ||
          mState == cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_DECODING_ATTRIBUTE_VALUE) {
         rval.mAttributes[currentAttributeKey] = currentAttributeValue;
      }
      else if (mState == cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE_ERROR) {
         rval.mState = false;
      }

      return rval;
   }

private:
   char mTagStart;
   char mTagEnd;
   char mAttributeDelimiter;
   char mKeyValueSeperator;

   std::string mErrorMessage;

   cg_tag_line_parser::CG_TAG_LINE_PARSE_STATE mState;

   cg_test_equal<unsigned char> mIsEqual;
   cg_test_equal<unsigned char> mIsMinus;
   cg_test_equal<unsigned char> mIsPlus;
   cg_test_equal<unsigned char> mIsSlash;

   std::list<cg_test_base<unsigned char> *> mTestAttributeValue;
   std::list<cg_test_base<unsigned char> *> mTestTagName;

   cg_lexing_rule<unsigned char>  mIsAttributeValueRule;
   cg_lexing_rule<unsigned char>  mIsTagNameRule;
};


#endif /* CG_TAG_PARSER_HPP_ */
