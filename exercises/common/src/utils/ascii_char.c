#include "utils/ascii_char.h"

#define CASE_FLAG (1 << 5)

/**
 * @brief Return true if the character is A-Z or a-z.
 */
int ascii_char_is_alpha(char c)
{
    if (('A' <= c && 'Z' >= c) || ('a' <= c && 'z' >= c)) {
        return 1;
    }

    return 0;
}

/**
 * @brief Return true if the character is a vowel.
 */
int ascii_char_is_vowel(char c)
{
    const char low_c = ascii_char_to_lower(c);

    if (('a' == low_c) || ('e' == low_c) || ('i' == low_c) ||
        ('o' == low_c) || ('u' == low_c) || ('y' == low_c)  ) {
        return 1;
    }

    return 0;
}

/**
 * @brief Return true if the character is 0-9.
 */
int ascii_char_is_numeric(char c)
{
    if ('0' <= c && '9' >= c) {
        return 1;
    }

    return 0;
}

/**
 * @brief Return E_TRUE if the character is A-Z, a-z, or 0-9.
 */
int ascii_char_is_alphanum(char c)
{
    if (ascii_char_is_alpha(c) || ascii_char_is_numeric(c)) {
        return 1;
    }

    return 0;
}

/**
 * @brief Return true if the character is a punctuation character.
 */
int ascii_char_is_punctuation(char c)
{
    if (ascii_char_is_terminal_punctuation(c)) {
        return 1;
    } else if ((',' == c) || ('\'' == c) || ('"' == c) || (';' == c) ||
               (':' == c) || ('-' == c)) {
        return 1;
    }

    return 0;
}

/**
 * @brief Return true if the character is a sentence terminating punctuation.
 */
int ascii_char_is_terminal_punctuation(char c)
{
    if ('.' == c || '?' == c || '!' == c) {
        return 1;
    }

    return 0;
}

/**
 * @brief Return true if the character is a white space character.
 */
int ascii_char_is_whitespace(char c)
{
    if ('\n' == c || '\t' == c || ' ' == c) {
        return 1;
    }

    return 0;
}

/**
 * @brief Convert the single digit integer to an ASCII digit.
 *
 * @param[in] d integer digit to convert to ASCII
 *
 * @return ASCII representation of the input integer digit
 *
 * NOTE: There is no error checking on the input. It is the caller's
 *       responsibility to check.
 */
char ascii_char_digit_to_ascii(uint8_t d)
{
    return (char)d + '0';
}

/**
 * @brief Convert provided character to lowercase.
 *
 * @param[in] c character to convert to lowercase
 *
 * @return lower case ASCII character
 *
 * NOTE: There is no error checking on the input. It is the caller's
 *       responsibility to check.
 */
char ascii_char_to_lower(char c)
{
    return c | CASE_FLAG;
}

/**
 * @brief Convert provided character to uppercase.
 *
 * @param[in] c character to convert to uppercase
 *
 * @return lower case ASCII character
 *
 * NOTE: There is no error checking on the input. It is the caller's
 *       responsibility to check.
 */
char ascii_char_to_upper(char c)
{
    return c & ~CASE_FLAG;
}