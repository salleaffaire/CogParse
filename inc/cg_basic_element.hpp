/*
 * cg_basic_element.hpp
 *
 *  Created on: Feb 2, 2014
 *      Author: lmartel
 */

#ifndef CG_BASIC_ELEMENT_HPP_
#define CG_BASIC_ELEMENT_HPP_

#include <list>
#include "cg_bit_extractor.hpp"

class cg_basic_element {
public:
   cg_basic_element()  {}
   virtual ~cg_basic_element() {}

   virtual std::list<cg_basic_element *> parse(cg_bit_buffer *bb) = 0;

private:

};

template <int S>
class uinteger : public cg_basic_element {
public:
   uinteger() : mValue(0) {}

   ~uinteger() {}

   std::list<cg_basic_element *> parse(cg_bit_buffer *bb) {
      mValue = bb->GetBits(S);
      return std::list<cg_basic_element *>();
   }

private:
   unsigned int    mValue;
};

#endif /* CG_BASIC_ELEMENT_HPP_ */
