#include "utils/ascii_char.h"

#include "types.h"

#define CASE_FLAG (1 << 5)

/**
 * @brief Return true if the character is A-Z or a-z.
 */
bool_t ascii_char_is_alpha(char c)
{
    bool_t result;

    if (('A' <= c && 'Z' >= c) || ('a' <= c && 'z' >= c)) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
}

/**
 * @brief Return true if the character is a vowel.
 */
bool_t ascii_char_is_vowel(char c)
{
    char   low_c;
    bool_t result;

    low_c = ascii_char_to_lower(c);

    if (('a' == low_c) || ('e' == low_c) || ('i' == low_c) || 
        ('o' == low_c) || ('u' == low_c) || ('y' == low_c)  ) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
}

/**
 * @brief Return true if the character is 0-9.
 */
bool_t ascii_char_is_numeric(char c)
{
    bool_t result;

    if ('0' <= c && '9' >= c) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
}

/**
 * @brief Return E_TRUE if the character is A-Z, a-z, or 0-9.
 */
bool_t ascii_char_is_alphanum(char c)
{
    bool_t result;
    
    if (E_TRUE == ascii_char_is_alpha(c) || E_TRUE == ascii_char_is_numeric(c)) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
}

/**
 * @brief Return true if the character is a punctuation character.
 */
bool_t ascii_char_is_punctuation(char c)
{
    bool_t result;

    if (E_TRUE == ascii_char_is_terminal_punctuation(c)) {
        result = E_TRUE;
    } else if ((',' == c) || ('\'' == c) || ('"' == c) || (';' == c) || 
               (':' == c) || ('-' == c)) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
}

/**
 * @brief Return true if the character is a sentence terminating punctuation.
 */
bool_t ascii_char_is_terminal_punctuation(char c)
{
    bool_t result;

    if ('.' == c || '?' == c || '!' == c) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
}

/**
 * @brief Return true if the character is a white space character.
 */
bool_t ascii_char_is_whitespace(char c)
{
    bool_t result;

    if ('\n' == c || '\t' == c || ' ' == c) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
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
char ascii_char_digit_to_ascii(u8_t d)
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