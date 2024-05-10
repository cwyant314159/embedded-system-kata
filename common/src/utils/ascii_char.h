#ifndef ASCII_CHAR_H
#define ASCII_CHAR_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool_t ascii_char_is_alpha(char c);
bool_t ascii_char_is_vowel(char c);
bool_t ascii_char_is_numeric(char c);
bool_t ascii_char_is_alphanum(char c);
bool_t ascii_char_is_punctuation(char c);
bool_t ascii_char_is_terminal_punctuation(char c);
bool_t ascii_char_is_whitespace(char c);
char ascii_char_digit_to_ascii(u8_t d);
char ascii_char_to_lower(char c);
char ascii_char_to_upper(char c);

#ifdef __cplusplus
}
#endif

#endif /* ASCII_CHAR_H */