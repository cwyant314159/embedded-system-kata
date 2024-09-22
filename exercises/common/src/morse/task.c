#include "morse/task.h"

#include "bsp/bsp.h"
#include "utils/ascii_char.h"
#include "types.h"

#include "morse/private/alphabet.h"
#include "morse/private/timings.h"

#define MAX_C_STR_LEN   (40)    /* handle strings of up to 40 chars */

/* In the worst case, a C string of all characters that have 5 symbols could be
   passed. For characters with 5 symbols, there will be 4 inter-symbol gaps plus
   a letter, word, or sentence gap.

   10 = 5 symbols + 4 inter-symbol gaps + 1  */
#define MAX_WAIT_TIMES (MAX_C_STR_LEN * 10)

/* The wait time buffer terminator value. When the processing loop hits this
   value, it knows that it has fully iterated through the wait time buffer */
#define WAIT_TIME_TERMINATOR (0u)

/**
 * @brief Morse module state machine state encoding.
 *
 * The morse module is implemented as a state machine that sits IDLE until a
 * C-string for encoding is provided. Once the string has been parsed into wait
 * times, the state machine transitions to the ENCODE-ing state. Encoding is the
 * process of toggling the LED at the rates determined by the C-string parsing.
 * Once the string has been fully encoded (blinked). The state machine will
 * either go back to IDLE and wait for another string, or if the module has been
 * configured to repeat the sequences, it will begin the ECODE state again.
 */
typedef enum morse_states
{
    E_STATE_IDLE,
    E_STATE_ENCODE,
} State_t;

/**
 * @brief Module's internal context structure
 */
typedef struct module_context
{
    bool_t repeat;                  /* do or don't repeat encoded message */
    u8_t   waits[MAX_WAIT_TIMES];   /* message as morse code wait times   */
    size_t waits_idx;               /* index into wait times array        */
    u8_t   ticks_left;              /* ticks left until index increments  */
} Context_t;

static State_t curr_state;
static Context_t ctx;

static void cstr_to_waits(Context_t *p_ctx, const char * const c_str);
static size_t pack_alphanum(char c, u8_t* wait_times);
static void reset_counters(Context_t *p_ctx);
static void reset_waits(Context_t *p_ctx);
static State_t idle_state(Context_t *p_ctx);
static State_t encode_state(Context_t *p_ctx);

/**
 * @brief Morse code module initialization
 */
void morse_task_init(void)
{
    curr_state = E_STATE_IDLE;
    reset_counters(&ctx);
    reset_waits(&ctx);
}

/**
 * @brief Morse code module task
 *
 * @NOTE: This task is expected to run once per 100ms.
 */
void morse_task(void)
{
    switch (curr_state)
    {
        case E_STATE_IDLE:
            curr_state = idle_state(&ctx);
            break;

        case E_STATE_ENCODE:
            curr_state = encode_state(&ctx);
            break;
        default:
            /* Should never get here but go back to IDLE just in case */
            curr_state = E_STATE_IDLE;
            break;
    }
}

/**
 * @brief Process a C-style string for the morse code module to encode
 *
 * @param[in] c_str_msg string containing the message to encode into morse code
 * @param[in] repeat when E_TRUE, configures the morse code module to repeatedly
 * output the message
 */
void morse_task_encode(const char * c_str_msg, bool_t repeat)
{
    /* Prep the context for the new message string */
    reset_counters(&ctx);
    reset_waits(&ctx);

    /* Convert the message string into wait times. */
    cstr_to_waits(&ctx, c_str_msg);
    ctx.repeat = repeat;

    /* Begin conversion */
    curr_state = E_STATE_ENCODE;
}

/**
 * @brief Return true if the morse module is actively encoding a message
 *
 * @note If the current message is configured to repeat, this will always return
 * true.
 *
 * @retval E_TRUE the morse module is encoding (blinking) a message
 * @retval E_FALSE the morse module is sitting idle and waiting for input
 */
bool_t morse_task_is_encoding(void)
{
    bool_t result;

    if (E_STATE_ENCODE == curr_state) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
}

/**
 * @brief Return true if the morse module is configured to repeat the current
 * message
 *
 * @retval E_TRUE the morse module repeats the current message indefinitely
 * @retval E_FALSE the morse module only encodes the message once
 */
bool_t morse_task_is_repeat(void)
{
    bool_t result;

    if (E_TRUE == ctx.repeat) {
        result = E_TRUE;
    } else {
        result = E_FALSE;
    }

    return result;
}

/**
 * @brief State machine IDLE state
 *
 * The IDLE state is simply a no-op where no morse encoding occurs.
 *
 * @param[inout] p_ctx pointer a module context structure
 *
 * @retval E_STATE_IDLE
 */
static State_t idle_state(Context_t *p_ctx)
{
    return E_STATE_IDLE;
}

/**
 * @brief State machine ENCODE state
 *
 * The ENCODE state will handle the count down and morse wait time array
 * traversal. Once the wait time array has been exhausted, the ENCODE state
 * will either go back to the beginning of the array to repeat the encoding, or
 * transition the state machine back to IDLE.
 *
 * @param[inout] p_ctx pointer a module context structure
 *
 * @retval E_STATE_IDLE
 */
static State_t encode_state(Context_t *p_ctx)
{
    State_t next_state;

    /* By default assume the state will stay in the ENCODE state */
    next_state = E_STATE_ENCODE;

    if (p_ctx->ticks_left > 0) {
        /* This symbol still has some time to wait. */
        p_ctx->ticks_left -= 1;
    } else {
        /* The ticks left has reached zero and it is time to step through the
           wait times array.

           When the value at the current index is 0 (or if the current index
           would point outside the wait time array), the message is over. The
           LED should be shut off and the counters reset. If the message has
           not been configured for repeats, go back to IDLE.

           If the value at the current index is not 0 (and the current index is
           within the wait time array), set the ticks left and move the index
           for the next time.
           */
        if (0 == p_ctx->waits[p_ctx->waits_idx] || p_ctx->waits_idx > MAX_WAIT_TIMES) {
            bsp_set_builtin_led(E_OFF);
            reset_counters(p_ctx);

            if (E_FALSE == p_ctx->repeat) {
                next_state = E_STATE_IDLE;
            }
        } else {
            bsp_toggle_builtin_led();
            p_ctx->ticks_left = p_ctx->waits[p_ctx->waits_idx] - 1;
            p_ctx->waits_idx += 1;
        }
    }

    return next_state;
}

/**
 * @brief Parse a C-style string into morse code LED flash wait times.
 *
 * @param[inout] p_ctx pointer a module context structure
 * @param[in] c_str C-style string to parse
 */
static void cstr_to_waits(Context_t *p_ctx, const char * const c_str)
{
    /*
     * NOTE: This function does no error checking. It is the caller's
     *       responsibility to ensure the output will fit in the buffer
     *       pointed to by the context's wait array.
     */

    size_t       idx;       /* index into the waits array                */
    const char  *curr_char; /* pointer to the current C string character */
    const char  *next_char; /* pointer to the current C string character */

    curr_char = c_str;
    next_char = curr_char + 1;
    idx       = 0;
    while (*curr_char != '\0') {

        if (E_TRUE == ascii_char_is_alphanum(*curr_char)) {
            idx += pack_alphanum(*curr_char, &p_ctx->waits[idx]);

            /* If the next character is another alphanumeric add the inter
               character gap. */
            if ('\0' != *next_char && E_TRUE == ascii_char_is_alphanum(*next_char)) {
                p_ctx->waits[idx] = MORSE_TIMING_CHAR_GAP;
                idx += 1;
            }
        } else if (E_TRUE == ascii_char_is_whitespace(*curr_char)) {
            p_ctx->waits[idx] = MORSE_TIMING_WORD_GAP;
            idx += 1;
        } else if (E_TRUE == ascii_char_is_terminal_punctuation(*curr_char)) {
            p_ctx->waits[idx] = MORSE_TIMING_SENTENCE_GAP;
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

/**
 * @brief Reset morse code context counters
 *
 * @param[inout] p_ctx pointer a module context structure
 */
void reset_counters(Context_t *p_ctx)
{
    /* ensure the LED output starts in the off state */
    bsp_set_builtin_led(E_OFF);

    /* reset processing counters */
    p_ctx->ticks_left = 0;
    p_ctx->waits_idx  = 0;
}

/**
 * @brief Reset morse code context wait time array
 *
 * @param[inout] p_ctx pointer a module context structure
 */
void reset_waits(Context_t *p_ctx)
{
    size_t i;   /* loop counter */

    /* Initialize wait time buffer. */
    for (i = 0; i < MAX_WAIT_TIMES; i += 1) {
        p_ctx->waits[i] = 0;
    }
}