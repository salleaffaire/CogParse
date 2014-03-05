/*
 * cg_lexer_base.hpp
 *
 *  Created on: Mar 1, 2014
 *      Author: lmartel
 */

#ifndef CG_LEXER_BASE_HPP_
#define CG_LEXER_BASE_HPP_

#include <string>
#include <map>
#include <list>
#include <regex>

#include "cg_char_tests.hpp"

// -------------------------------------------------------------------
// Test classes
// -------------------------------------------------------------------

template <class T>
class cg_test_base {
public:
   cg_test_base() {}
   virtual ~cg_test_base() {}

   virtual bool test(T x) = 0;
};

template <class T>
class cg_test_range : public cg_test_base<T> {
public:
   cg_test_range(T l, T u) : mLow(l), mUp(u) {}
   virtual ~cg_test_range() {}

   bool test(T c) { return ((c >= mLow) && (c <= mUp)) ? true : false; }

private:
   T mLow;
   T mUp;
};

template <class T>
class cg_test_equal : public cg_test_base<T> {
public:
   cg_test_equal(T c) : mC(c) {}
   virtual ~cg_test_equal() {}

   bool test(T c) { return c == mC; }

private:
   T mC;
};

template <class T>
class cg_test_notequal : public cg_test_base<T> {
public:
   cg_test_notequal(T c) : mC(c) {}
   virtual ~cg_test_notequal() {}

   bool test(T c) { return c != mC; }

private:
   T mC;
};

// -------------------------------------------------------------------
// Lexing rule class
// -------------------------------------------------------------------

class cg_lexing_rule {
public:
   typedef bool (*lexing_rule_fn)(bool, bool);

   static lexing_rule_fn        mOr;
   static lexing_rule_fn        mAnd;

   cg_lexing_rule() {}
   ~cg_lexing_rule() {}

   lexing_rule_fn                            mFn;
   bool                                      mInit;
   std::list<cg_test_base<unsigned char> *>  mTestList;
};


cg_lexing_rule::lexing_rule_fn cg_lexing_rule::mOr = *[](bool in, bool r) -> bool {
   return in || r;
};

cg_lexing_rule::lexing_rule_fn cg_lexing_rule::mAnd = *[](bool in, bool r) -> bool {
   return in && r;
};

// -------------------------------------------------------------------



template <class T>
bool cg_test(T c, std::list<cg_test_base<T> *> &t_l, bool init, bool (*f)(bool, bool)) {
   bool rval = init;

   for (typename std::list<cg_test_base<T> *>::iterator it = t_l.begin();
        (it != t_l.end());
        ++it) {
      rval = f((*it)->test(c), rval);
   }

   return rval;
}

bool cg_test(unsigned char c, cg_lexing_rule &lr) {
   return cg_test<unsigned char>(c, lr.mTestList, lr.mInit, lr.mFn);
}


class cg_lexer_base {
public:
   cg_lexer_base() :
      // Pre-define ranges
      // -------------------------------------------------------------------
      mIsInPrintableRange(0x21, 0x7E),
      mIsInUppercaseRange(0x41, 0x5A),   // Upper case letters [A-Z]
      mIsInLowercaseRange(0x61, 0x7A),   // Lower case letters [a-z]
      mIsInDecimalRange(0x30, 0x39),     // Decimal digits     [0-9]
      mIsInOctalRange(0x30, 0x37),       // Octal digits       [0-7]
      mIsInHexupRange(0x41, 0x46),       // Upper case alpha hex digits [A-F]
      mIsInHexlowRange(0x61, 0x66),       // Lower case alpha hex digits [a-f]
      mIsNotDoubleQuote('"')
   {

      // -------------------------------------------------------------------
      mTestDecimal.push_back(&mIsInDecimalRange);  //    in decimal range

      // -------------------------------------------------------------------
      mTestOctal.push_back(&mIsInOctalRange);      //    in octal range

      // -------------------------------------------------------------------
      mTestHex.push_back(&mIsInDecimalRange);      //    in decimal range
      mTestHex.push_back(&mIsInHexupRange);        // or in hex up range
      mTestHex.push_back(&mIsInHexlowRange);       // or in hex low range

      // -------------------------------------------------------------------
      mTestLetter.push_back(&mIsInUppercaseRange); //    in upper case range
      mTestLetter.push_back(&mIsInLowercaseRange); // or in lower case range

      // -------------------------------------------------------------------
      mTestAlphaNum.push_back(&mIsInDecimalRange);   //    in decimal range
      mTestAlphaNum.push_back(&mIsInUppercaseRange); // or in upper case range
      mTestAlphaNum.push_back(&mIsInLowercaseRange); // or in lower case range

      // -------------------------------------------------------------------
      mTestStringLiteral.push_back(&mIsInPrintableRange); //     in decimal range
      mTestStringLiteral.push_back(&mIsNotDoubleQuote);   // and not equal to "

      // Default rules
      // -------------------------------------------------------------------
      // -------------------------------------------------------------------
      mIsDecimalRule.mFn       = cg_lexing_rule::mOr;
      mIsDecimalRule.mInit     = false;
      mIsDecimalRule.mTestList = mTestDecimal;

      mIsOctalRule.mFn         = cg_lexing_rule::mOr;
      mIsOctalRule.mInit       = false;
      mIsOctalRule.mTestList   = mTestOctal;

      mIsHexRule.mFn           = cg_lexing_rule::mOr;
      mIsHexRule.mInit         = false;
      mIsHexRule.mTestList     = mTestHex;

      mIsLetterRule.mFn        = cg_lexing_rule::mOr;
      mIsLetterRule.mInit      = false;
      mIsLetterRule.mTestList  = mTestLetter;

      mIsAlphaNumRule.mFn        = cg_lexing_rule::mOr;
      mIsAlphaNumRule.mInit      = false;
      mIsAlphaNumRule.mTestList  = mTestAlphaNum;

      mIsStringLiteralRule.mFn        = cg_lexing_rule::mAnd;
      mIsStringLiteralRule.mInit      = true;
      mIsStringLiteralRule.mTestList  = mTestStringLiteral;

   }
   virtual ~cg_lexer_base() {}

   void push_rule(std::string rule_name, std::list<cg_test_base<unsigned char> *> rule) {
      mUserDefinedTestMap[rule_name] = rule;
   }

protected:

   // --------------------------------------------------------
   bool decode_string_literal(std::string::iterator &it, const std::string::iterator &end, std::string &s) {
      bool rval = true;

      if (*it == '"') {
         // Swallow the opening double-quote, update the iterator
         s += *it++;

         while (cg_test(*it, mIsStringLiteralRule)) {
            s += *it++;
         }

         // Swallow the closing double-quote, do not update the iterator
         s += *it;
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
   // --------------------------------------------------------
   bool decode_number(std::string::iterator &it, const std::string::iterator &end, std::string &s) {
      bool rval = true;

      // Octal or Hex
      if (*it == '0') {
         // We're hex
         if ((*it == 'x') || ((*it == 'X'))) {
            // Swallow the 'x' or 'X'
            s += *it++;

            // Decode HEX
            decode_hex(it, end,  s);
         }
         // We're Octal
         else {
            // Swallow the 0
            s += *it++;

            // Decode OCT
            decode_oct(it, end, s);
         }
      }
      else {
         // Decode DIG
         decode_dec(it, end, s);
      }

      return rval;
   }

   // Default deciding functions - "decode while test is true"
   bool decode_while(std::string::iterator &it,
                     const std::string::iterator &end,
                     std::string &s,
                     cg_lexing_rule &lr
                     ) {
      bool rval = true;

      while (cg_test(*it, lr) && it != end)  {
         s += *it++;
      }

      return rval;
   }

   // Useful ones using defauls tests
   bool decode_dec(std::string::iterator &it, const std::string::iterator &end, std::string &s) {
      return decode_while(it, end, s, mIsDecimalRule);
   }

   bool decode_oct(std::string::iterator &it, const std::string::iterator &end,  std::string &s) {
      return decode_while(it, end, s, mIsOctalRule);
   }

   bool decode_hex(std::string::iterator &it, const std::string::iterator &end,  std::string &s) {
      return decode_while(it, end, s, mIsHexRule);;
   }

   // Default useful caracter ranges
   // ----------------------------------------------------------------------------
   cg_test_range<unsigned char>             mIsInPrintableRange;
   cg_test_range<unsigned char>             mIsInUppercaseRange;
   cg_test_range<unsigned char>             mIsInLowercaseRange;
   cg_test_range<unsigned char>             mIsInDecimalRange;
   cg_test_range<unsigned char>             mIsInOctalRange;
   cg_test_range<unsigned char>             mIsInHexupRange;
   cg_test_range<unsigned char>             mIsInHexlowRange;

   cg_test_notequal<unsigned char>          mIsNotDoubleQuote;

   // Default test lists
   // ----------------------------------------------------------------------------
   std::list<cg_test_base<unsigned char> *> mTestLetter;
   std::list<cg_test_base<unsigned char> *> mTestDecimal;

   std::list<cg_test_base<unsigned char> *> mTestOctal;
   std::list<cg_test_base<unsigned char> *> mTestHex;
   std::list<cg_test_base<unsigned char> *> mTestAlphaNum;

   std::list<cg_test_base<unsigned char> *> mTestStringLiteral;

   // Default rules
   // ----------------------------------------------------------------------------
   cg_lexing_rule mIsLetterRule;
   cg_lexing_rule mIsDecimalRule;

   cg_lexing_rule mIsOctalRule;
   cg_lexing_rule mIsHexRule;
   cg_lexing_rule mIsAlphaNumRule;

   cg_lexing_rule mIsStringLiteralRule;


   // User defined rules
   // -----------------------------------------------------------------------------
   std::map<std::string, std::list<cg_test_base<unsigned char> *>> mUserDefinedTestMap;
};



#endif /* CG_LEXER_BASE_HPP_ */
