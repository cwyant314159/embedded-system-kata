#include "bsp/bsp.h"
#include "types.h"

/* Morse encoding in 100's of milliseconds */
#define DOT          (1)        /* 100ms  */
#define DASH         (4 * DOT)  /* 400ms  */
#define SYM_GAP      (1 * DOT)  /* 100ms  */
#define CHAR_GAP     (5 * DOT)  /* 500ms  */
#define WORD_GAP     (7 * DOT)  /* 700ms  */
#define SENTENCE_GAP (15 * DOT) /* 1500ms */

/* Number of morse elements in an SOS message */
#define NUM_MORSE_ELEMENTS  (18)

static const u8_t SOS_MORSE_TICKS[NUM_MORSE_ELEMENTS] = {
    DOT, SYM_GAP, DOT, SYM_GAP, DOT,    /* S */
    CHAR_GAP,
    DASH, SYM_GAP, DASH, SYM_GAP, DASH, /* O */
    CHAR_GAP,
    DOT, SYM_GAP, DOT, SYM_GAP, DOT,    /* S */
    SENTENCE_GAP
};


static volatile size_t morse_index;
static volatile u8_t ticks_left;

static void bsp_timer_isr_callback(void);


/**
 * @brief SOS
 *
 * Blink SOS in morse code with the LED.
 */
int main(void)
{
    /* Initialize the board support (e.g. the LED) */
    bsp_init();

    /* In the timer interrupt, we are going to iterate through the array of
       morse times and decrement a counter (a tick). Every time that counter
       reaches 0, the LED will toggle. To prime this process, we are going to
       start with the LED  off, and start with the last element of the morse
       array (SENTENCE_GAP) */
    bsp_set_builtin_led(E_OFF);
    morse_index = NUM_MORSE_ELEMENTS - 1;
    ticks_left  = SOS_MORSE_TICKS[morse_index];
    bsp_register_timer_isr_callback(bsp_timer_isr_callback);

    /* Enable interrupts now, so when the timer is configured and started the
       first interrupt is not missed */
    bsp_enable_interrupts();

    /* The morse code array contains delay times in 100s of milliseconds. */
    bsp_set_timer_period_msec(100u);

    while (1) {
        /* Nothing to do */
    }

    return 0; /* Satisfy compiler. Should never get here */
}

/**
 * @brief BSP timer interrupt callback
 *
 * This decrements the ticks remaining for the current morse symbol. Once the
 * tick count reaches zero, the array index is advanced for the next morse 
 * element. When all the elements have been "toggled" out, the index wraps back
 * to the beginning.
 */
static void bsp_timer_isr_callback(void)
{
    ticks_left -= 1;

    /* When ticks reaches 0 (or overflows and becomes bigger than the requested
       ticks), toggle the LED and prepare for the next morse element. */
    if (0 == ticks_left || ticks_left > SOS_MORSE_TICKS[morse_index]) {
        bsp_toggle_builtin_led();

        /* Move to the next morse element. Rollover to 0 and handle the
           unfortunate case where the index is greater than expected. */
        morse_index += 1;
        if (NUM_MORSE_ELEMENTS <= morse_index) {
            morse_index = 0;
        }

        ticks_left = SOS_MORSE_TICKS[morse_index];
    }
}
