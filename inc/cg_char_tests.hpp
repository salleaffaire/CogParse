/*
 * cg_char_tests.hpp
 *
 *  Created on: Feb 27, 2014
 *      Author: luc.martel
 */

#ifndef CG_CHAR_TESTS_HPP_
#define CG_CHAR_TESTS_HPP_

bool cg_char_is_upper(unsigned char x) { return ((x >= 0x41) && (x <= 0x5A)) ? true : false ; }
bool cg_char_is_lower(unsigned char x) { return ((x >= 0x61) && (x <= 0x7A)) ? true : false ; }
bool cg_char_is_num(unsigned char x)   { return ((x >= 0x30) && (x <= 0x39)) ? true : false ; }

// Everything but exclude 0x22 = '"' : the double quote
bool cg_char_is_stringliteralchar(unsigned char x) {return ((x >= 0x21) && (x <= 0x7E) && (x != 0x22)) ? true : false; }

bool cg_char_is_alphanum(unsigned char x) { return (cg_char_is_upper(x) || cg_char_is_lower(x) || cg_char_is_num(x)); }

#endif /* CG_CHAR_TESTS_HPP_ */
