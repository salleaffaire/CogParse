/*
 * cg_lexer_base.cpp
 *
 *  Created on: Mar 5, 2014
 *      Author: luc.martel
 */

#include "cg_lexer_base.hpp"

lexing_rule_fn lr_or = *[](bool in, bool r) -> bool {
   return in || r;
};

lexing_rule_fn lr_and = *[](bool in, bool r) -> bool {
   return in && r;
};
