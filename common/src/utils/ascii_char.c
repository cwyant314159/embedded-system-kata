#include "utils/ascii_char.h"

#include "types.h"

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