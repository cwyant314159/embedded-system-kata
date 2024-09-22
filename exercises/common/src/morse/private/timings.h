#ifndef MORSE_PRIVATE_TIMINGS_H
#define MORSE_PRIVATE_TIMINGS_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************/
/* NOTE: All timings are given in 100s of milliseconds*/
/******************************************************/

/* All morse timing is based off DOT times. For application, a DOT time is 100
   milliseconds. */
#define MORSE_TIMING_DOT          (1u)                     /* . */
#define MORSE_TIMING_DASH         (4 * MORSE_TIMING_DOT)   /* - */
#define MORSE_TIMING_SYM_GAP      (1 * MORSE_TIMING_DOT)   /* between dots and dash in letter */
#define MORSE_TIMING_CHAR_GAP     (5 * MORSE_TIMING_DOT)   /* between letters of a word */
#define MORSE_TIMING_WORD_GAP     (7 * MORSE_TIMING_DOT)   /* between words of a sentence */
#define MORSE_TIMING_SENTENCE_GAP (15 * MORSE_TIMING_DOT)  /* between sentences */

#ifdef __cplusplus
}
#endif

#endif /* MORSE_PRIVATE_TIMINGS_H */