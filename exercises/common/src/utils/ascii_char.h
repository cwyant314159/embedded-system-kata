#ifndef ASCII_CHAR_H
#define ASCII_CHAR_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int ascii_char_is_alpha(char c);
int ascii_char_is_vowel(char c);
int ascii_char_is_numeric(char c);
int ascii_char_is_alphanum(char c);
int ascii_char_is_punctuation(char c);
int ascii_char_is_terminal_punctuation(char c);
int ascii_char_is_whitespace(char c);
char ascii_char_digit_to_ascii(uint8_t d);
char ascii_char_to_lower(char c);
char ascii_char_to_upper(char c);

#ifdef __cplusplus
}
#endif

#endif /* ASCII_CHAR_H */