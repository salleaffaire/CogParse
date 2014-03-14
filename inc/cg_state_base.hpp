/*
 * cg_state_base.hpp
 *
 *  Created on: Mar 14, 2014
 *      Author: luc.martel
 */

#ifndef CG_STATE_BASE_HPP_
#define CG_STATE_BASE_HPP_

#include <vector>

class cg_state_base {
public:
   cg_state_base() {}
   virtual ~cg_state_base() {}

   virtual cg_state_base *execute(std::vector<cg_state_base *> &mStateVector) = 0;
};


#endif /* CG_STATE_BASE_HPP_ */
