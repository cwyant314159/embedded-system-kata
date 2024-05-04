#include "morse.h"

#include "bsp/bsp.h"
#include "utils/ascii_char.h"
#include "types.h"

#include "morse_alphabet.h"
#include "morse_timings.h"

#define MAX_C_STR_LEN   (40)    /* handle strings of up to 40 chars */

/* In the worst case, a C string of all characters that have 5 symbols could be
   passed. For characters with 5 symbols, there will be 4 inter-symbol gaps plus
   a letter, word, or sentence gap.

   10 = 5 symbols + 4 inter-symbol gaps + 1  */
#define MAX_WAIT_TIMES (MAX_C_STR_LEN * 10)

/* The wait time buffer terminator value. When the processing loop hits this
   value, it knows that it has fully iterated through the wait time buffer */
#define WAIT_TIME_TERMINATOR (0u)

static const char * const msg = "Hello, Morse!";

static u8_t   morse_wait_times[MAX_WAIT_TIMES];
static size_t morse_time_idx;
static u8_t   ticks_left;

static void cstr_to_waits(const char * const c_str, u8_t *p_times);
static size_t pack_alphanum(char c, u8_t* wait_times);

/**
 * @brief Morse code module initialization
 */
void morse_init(void)
{
    size_t i;

    bsp_set_builtin_led(E_OFF);

    /* Initialize wait time buffer. */
    for (i = 0; i < MAX_WAIT_TIMES; i += 1) {
        morse_wait_times[i] = 0;
    }

    /* Convert the message string into wait times. */
    cstr_to_waits(msg, morse_wait_times);
    ticks_left     = 0;
    morse_time_idx = 0;
}

/**
 * @brief Morse code module task
 * 
 * @NOTE: This task is expected to run once per 100ms.
 */
void morse_task(void)
{
    if (ticks_left > 0) {
        ticks_left -= 1;
    } else {
        bsp_toggle_builtin_led();
        ticks_left = morse_wait_times[morse_time_idx] - 1;

        /* move wait time index for next timeout. Handle overflow. */
        morse_time_idx += 1;
        if (0 == morse_wait_times[morse_time_idx] || morse_time_idx > MAX_WAIT_TIMES) {
            morse_time_idx = 0;
        }
    }
}

/**
 * @brief Parse a C-style string into morse code LED flash wait times.
 * 
 * @param[in] c_str C-style string to parse
 * @param[out] p_times pointer to buffer memory for morse timings
 */
static void cstr_to_waits(const char * const c_str, u8_t *p_times)
{
    /*
     * NOTE: This function does no error checking. It is the caller's
     *       responsibility to ensure the output will fit in the buffer
     *       pointed to by p_times.
     */

    size_t       idx;       /* index into the times array                */
    const char  *curr_char; /* pointer to the current C string character */
    const char  *next_char; /* pointer to the current C string character */
    
    curr_char = c_str;
    next_char = curr_char + 1;
    idx       = 0;
    while (*curr_char != '\0') {

        if (E_TRUE == ascii_char_is_alphanum(*curr_char)) {
            idx += pack_alphanum(*curr_char, &p_times[idx]);
            
            /* If the next character is another alphanumeric add the inter
               character gap. */
            if ('\0' != *next_char && E_TRUE == ascii_char_is_alphanum(*next_char)) {
                p_times[idx] = MORSE_TIMING_CHAR_GAP;
                idx += 1;
            }
        } else if (E_TRUE == ascii_char_is_whitespace(*curr_char)) {
            p_times[idx] = MORSE_TIMING_WORD_GAP;
            idx += 1;
        } else if (E_TRUE == ascii_char_is_terminal_punctuation(*curr_char)) {
            p_times[idx] = MORSE_TIMING_SENTENCE_GAP;
            idx += 1;
        } else {
            /* Ignore all other characters (e.g. comma, carriage return, non-
               printables, etc.) */
        }

        curr_char += 1;
        next_char += 1;
    }
}

/**
 * @brief Pack morse code alphanumeric character wait times into a buffer of 
 * wait times.
 * 
 * @param[in] c character to parse
 * @param[out] out_times pointer to buffer holding morse code timings.
 * 
 * @return number of timing values added to output buffer
 */
static size_t pack_alphanum(char c, u8_t* out_times)
{
    size_t             sym_idx;       /* symbol time loop counter  */
    size_t             time_idx;      /* out time loop counter     */
    const MorseChar_t *p_morse_char;  /* converted Morse character */
    
    if (E_TRUE == ascii_char_is_alpha(c)) {
        p_morse_char = &MORSE_ALPHA_TABLE[ALPHA_CHAR_TO_IDX(c)];
    } else if (E_TRUE == ascii_char_is_numeric(c)) {
        p_morse_char = &MORSE_NUMERIC_TABLE[NUM_CHAR_TO_IDX(c)];
    } else {
        p_morse_char = NULL_PTR;
        bsp_error_trap();
    }

    sym_idx  = 0;
    time_idx = 0;
    while (MORSE_CHAR_TERMINATOR != p_morse_char->symbol[sym_idx]) {
        out_times[time_idx] = p_morse_char->symbol[sym_idx];
        time_idx += 1;

        /* Only put an inter-symbol gap if there is a next symbol */
        if (MORSE_CHAR_TERMINATOR != p_morse_char->symbol[sym_idx + 1]) {
            out_times[time_idx] = MORSE_TIMING_SYM_GAP;
            time_idx += 1;
        }

        sym_idx += 1;
    }

    return time_idx;
}