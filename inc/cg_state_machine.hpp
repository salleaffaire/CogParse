/*
 * cg_state_machine.hpp
 *
 *  Created on: Mar 14, 2014
 *      Author: luc.martel
 */

#ifndef CG_STATE_MACHINE_HPP_
#define CG_STATE_MACHINE_HPP_

#include "cg_state_base.hpp"

class cg_state_machine {
public:
   enum Status {
      READY,
      RUNNING,
      PAUSED,
      DONE,
      ERROR
   };

   cg_state_machine() : mStatus(READY), mNumStates(0) {}
   cg_state_machine(unsigned int ns) : mStatus(READY), mNumStates(ns)
   {
      mStateVector.resize(ns);

      std::vector<cg_state_base *>::iterator it = mStateVector.begin();
      for (;it != mStateVector.end(); ++it) {
         *it = 0;
      }
   }
   ~cg_state_machine()
   {
      std::vector<cg_state_base *>::iterator it = mStateVector.begin();
      for (;it != mStateVector.end(); ++it) {
         if (*it) {
            delete *it;
         }
      }
   }

   void set_state(cg_state_base *s, unsigned int index) {
      if (index >= mNumStates) {
         std::cout << "Invalid Index : " << index << std::endl;
      }
      else {
         mStateVector[index] = s;
      }
   }

   int run() {
      int rval = 0;
      mStatus = RUNNING;
      cg_state_base *lCurrentState = mStateVector[0];
      while (lCurrentState != mStateVector[mNumStates-1]) {
         lCurrentState = lCurrentState->execute(mStateVector);
         if (lCurrentState == 0) {
            mStatus = ERROR;
            break;
         }
      }
      if (mStatus != ERROR) {
         lCurrentState->execute(mStateVector);
         mStatus = DONE;
      }
      return rval;
   }

private:
   Status                       mStatus;
   unsigned int                 mNumStates;
   std::vector<cg_state_base *> mStateVector;

};


#endif /* CG_STATE_MACHINE_HPP_ */
