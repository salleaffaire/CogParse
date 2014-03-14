/*
 * cg_state_tests.hpp
 *
 *  Created on: Mar 14, 2014
 *      Author: luc.martel
 */

#ifndef CG_STATE_TESTS_HPP_
#define CG_STATE_TESTS_HPP_

#include <iostream>
#include "cg_state_base.hpp"

// Forward declaration
class cg_st_tts_start;
class cg_st_tts_query;
class cg_st_tts_end;

class cg_st_tts_start : public cg_state_base {
public:
   cg_st_tts_start() {}
   ~cg_st_tts_start() {}

   cg_state_base *execute(std::vector<cg_state_base *> &sv) {
      std::cout << "Starting ... " << std::endl;
      return sv[1];
   }
};

class cg_st_tts_query : public cg_state_base {
public:
   cg_st_tts_query() {}
   ~cg_st_tts_query() {}

   cg_state_base *execute(std::vector<cg_state_base *> &sv) {
      cg_state_base *rval;

      char a;
      std::cout << "Enter char: ";
      std::cin >> a;
      switch (a) {
      case 'A':
         rval = sv[0];
         break;
      case 'B':
         rval = sv[1];
         break;
      default:
         rval = sv[2];
         break;
      }

      return rval;
   }
};

class cg_st_tts_end : public cg_state_base {
public:
   cg_st_tts_end() {}
   ~cg_st_tts_end() {}

   cg_state_base *execute(std::vector<cg_state_base *> &sv) {
      std::cout << "End ... " << std::endl;
      return sv[2];
   }
};


#endif /* CG_STATE_TESTS_HPP_ */
