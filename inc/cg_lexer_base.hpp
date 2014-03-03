/*
 * cg_lexer_base.hpp
 *
 *  Created on: Mar 1, 2014
 *      Author: lmartel
 */

#ifndef CG_LEXER_BASE_HPP_
#define CG_LEXER_BASE_HPP_

#include <string>
#include <list>
#include <map>
#include <regex>

#include "cg_char_tests.hpp"

class cg_lexer_base {
public:
   cg_lexer_base() :
      // Define ranges
      mUppercaseRange(0x41, 0x5A),
      mLowercaseRange(0x61, 0x7A),
      mDecimalRange(0x30, 0x39),
      mOctalRange(0x30, 0x37),
      mHexupRange(0x41, 0x46),
      mHexlowRange(0x61, 0x66) {

      // -----------------------------------------
      mTestDecimal.push_back(&mDecimalRange);

      // -----------------------------------------
      mTestOctal.push_back(&mOctalRange);

      // -----------------------------------------
      mTestHex.push_back(&mDecimalRange);
      mTestHex.push_back(&mHexupRange);
      mTestHex.push_back(&mHexlowRange);

      // -----------------------------------------
      mTestLetter.push_back(&mUppercaseRange);
      mTestLetter.push_back(&mLowercaseRange);

      // -----------------------------------------
      mTestAlphaNum.push_back(&mDecimalRange);
      mTestAlphaNum.push_back(&mUppercaseRange);
      mTestAlphaNum.push_back(&mLowercaseRange);
   }
   virtual ~cg_lexer_base() {}


protected:

   bool decode_string_literal(std::string::iterator &it, std::string &s) {
      bool rval = true;

      if (*it == '"') {
         // Use the opening double-quote
         s += *it++;

         while (cg_char_is_stringliteralchar(*it)) {
            s += *it++;
         }

         // Use the closing double-quote
         s += *it++;
      }
      else {
         rval = false;
      }

      return rval;
   }

   // Decode number
   // 23            -> Decimal
   // 0X17 or 0x17  -> Hex
   // 027           -> Octal

   bool decode_number(std::string::iterator &it, std::string &s) {
      bool rval = true;

      // Octal or Hex
      if (*it == '0') {
         // We're hex
         if ((*it == 'x') || ((*it == 'X'))) {
            // Swallow the 'x' or 'X'
            s += *it++;

            // Decode HEX
            decode_hex(it, s);

         }
         // We're Octal
         else {
            // Swallow the 0
            s += *it++;

            // Decode OCT
            decode_oct(it, s);
         }

      }
      else {
         // Decode DIG
         decode_dec(it, s);
      }

      return rval;
   }

   // Default deciding functions - "decode while test is true"
   bool decode_while(std::string::iterator &it,
                     std::string &s,
                     std::list<cg_test_base<unsigned char> *> &test) {
      bool rval = true;

      //while (cg_char_is_dec(*it)) {
      while (cg_test<unsigned char>(*it, test))  {
         s += *it++;
      }

      return rval;
   }

   // Useful ones using defauls tests
   bool decode_dec(std::string::iterator &it, std::string &s) {
      return decode_while(it, s, mTestDecimal);
   }

   bool decode_oct(std::string::iterator &it, std::string &s) {
      return decode_while(it, s, mTestOctal);
   }

   bool decode_hex(std::string::iterator &it, std::string &s) {
      return decode_while(it, s, mTestHex);
   }

   // Default tests -
   std::list<cg_test_base<unsigned char> *> mTestLetter;
   std::list<cg_test_base<unsigned char> *> mTestDecimal;

   std::list<cg_test_base<unsigned char> *> mTestOctal;
   std::list<cg_test_base<unsigned char> *> mTestHex;
   std::list<cg_test_base<unsigned char> *> mTestAlphaNum;

   // Default useful range tests
   cg_test_range<unsigned char>             mUppercaseRange;
   cg_test_range<unsigned char>             mLowercaseRange;
   cg_test_range<unsigned char>             mDecimalRange;
   cg_test_range<unsigned char>             mOctalRange;
   cg_test_range<unsigned char>             mHexupRange;
   cg_test_range<unsigned char>             mHexlowRange;

   std::map

};



#endif /* CG_LEXER_BASE_HPP_ */
