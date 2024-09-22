#ifndef MORSE_PRIVATE_ALPHABET_H
#define MORSE_PRIVATE_ALPHABET_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Convert an ASCII alphabet character to an index into the alphabet table. This
   does no error checking so be careful.

   The AND'ing ensures the ASCII alpha character is uppercase.
*/
#define ALPHA_CHAR_TO_IDX(c)    (((c) & ~(1<<5)) - 'A')

/* Convert an ASCII numeric character to an index into the number table. This
   does no error checking so be careful. */
#define NUM_CHAR_TO_IDX(c)      ((c) - '0')

/* MorseChar_t time buffer terminator symbol. This denotes the end of a morse
   code character. This is necessary since most morse code characters don't have
   have the same number of symbols (e.g. A vs W). */
#define MORSE_CHAR_TERMINATOR   (0)

/**
 * @brief Representation of a morse code character (dots, dashes and inter
 * symbol gaps).
 *
 * A morse code character is simply an array of delay times representing the
 * "on" times for symbols and "off" times for inter symbol gaps.
 *
 * Since not all morse characters are the same symbol length, a terminator value
 * is needed so that processing logic can know when to move on to the next
 * character. The value 0 is chosen since all morse code symbols require some
 * amount of time for expression.
 *
 * NOTE: To support the morse code alphabet and numeric table generation,
 *       processing logic should ignore inter symbol gap timings if they are
 *       followed by the terminator. However, it should be noted that dot and
 *       dash times alway occur on even number indices within the time array.
 *       This means that the logic to check the end of a character should be
 *       similar to the following pseudo-code:
 *
 *       (is_odd(idx) && is_0(c.time[idx+1])) || is_0(c.time[idx]
 */
typedef struct morse_character
{
    /* To support the longest possible morse code character a buffer of 11
       times is required. 5 for the symbols, 4 for the inter symbol gpas, 1 for
       the inter symbol gap quirk mentioned above, and 1 for the the terminator
       value.*/
    u8_t symbol[11];
} MorseChar_t;

/* The alphabet of morse code characters. Use ALPHA_CHAR_TO_IDX for access. */
extern const MorseChar_t MORSE_ALPHA_TABLE[26];

/* 0 - 9 of morse code characters. Use NUM_CHAR_TO_IDX for access. */
extern const MorseChar_t MORSE_NUMERIC_TABLE[10];

#ifdef __cplusplus
}
#endif



#endif /* MORSE_PRIVATE_ALPHABET_H */