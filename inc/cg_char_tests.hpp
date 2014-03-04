/*
 * cg_char_tests.hpp
 *
 *  Created on: Feb 27, 2014
 *      Author: luc.martel
 */

#ifndef CG_CHAR_TESTS_HPP_
#define CG_CHAR_TESTS_HPP_

// Templated Range (not used, but still cool - will work on it later)
// -------------------------------------------------------------------

template <class T, int L, int U>
class cg_t_range {
public:
   static bool test(T c) { return ((c >= L) && (c <= U)) ? true : false; }
};

template <class T, class R>
bool cg_t_is_in_range(T c) {
   return R::test(c);
};

typedef cg_t_range<unsigned char, 0x41, 0x5A> cg_uppercase_range;
typedef cg_t_range<unsigned char, 0x61, 0x7A> cg_lowercase_range;
typedef cg_t_range<unsigned char, 0x30, 0x39> cg_decimal_range;
typedef cg_t_range<unsigned char, 0x30, 0x37> cg_octal_range;
typedef cg_t_range<unsigned char, 0x41, 0x46> cg_uphex_range;
typedef cg_t_range<unsigned char, 0x61, 0x66> cg_lowhex_range;

// ---------------------------------------------------------------

bool cg_char_is_upper (unsigned char c) {
   return cg_t_is_in_range<unsigned char, cg_uppercase_range>(c);
}
bool cg_char_is_lower (unsigned char c) {
   return cg_t_is_in_range<unsigned char, cg_lowercase_range>(c);
}
bool cg_char_is_dec (unsigned char c) {
   return cg_t_is_in_range<unsigned char, cg_decimal_range>(c);
}
bool cg_char_is_oct (unsigned char c) {
   return cg_t_is_in_range<unsigned char, cg_octal_range>(c);
}

bool cg_char_is_hex(unsigned char c) {
   return (cg_char_is_dec(c)                                  ||
           cg_t_is_in_range<unsigned char, cg_uphex_range>(c) ||
           cg_t_is_in_range<unsigned char, cg_uphex_range>(c)) ? true : false ;
}

// Everything but exclude 0x22 = '"' : the double quote
bool cg_char_is_stringliteralchar(unsigned char x) {return ((x >= 0x21) && (x <= 0x7E) && (x != 0x22)) ? true : false; }

bool cg_char_is_alphanum(unsigned char x) { return (cg_char_is_upper(x) || cg_char_is_lower(x) || cg_char_is_dec(x)); }

#endif /* CG_CHAR_TESTS_HPP_ */
