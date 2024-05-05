#include "morse/private/alphabet.h"

#include "morse/private/timings.h"

/* Macro short hands */
#define DOT         MORSE_TIMING_DOT
#define DASH        MORSE_TIMING_DASH
#define A_2_IDX(c)  ALPHA_CHAR_TO_IDX((c))
#define N_2_IDX(c)  NUM_CHAR_TO_IDX((c))

/**
 * @brief Morse code alphabet lookup table
 */
const MorseChar_t MORSE_ALPHA_TABLE[26] = {
    [A_2_IDX('A')] = { .symbol = {DOT,  DASH}             },
    [A_2_IDX('B')] = { .symbol = {DASH, DOT,  DOT,  DOT}  },
    [A_2_IDX('C')] = { .symbol = {DASH, DOT,  DASH, DOT}  },
    [A_2_IDX('D')] = { .symbol = {DASH, DOT,  DOT}        },
    [A_2_IDX('E')] = { .symbol = {DOT}                    },
    [A_2_IDX('F')] = { .symbol = {DOT,  DOT,  DASH, DOT}  },
    [A_2_IDX('G')] = { .symbol = {DASH, DASH, DOT}        },
    [A_2_IDX('H')] = { .symbol = {DOT,  DOT,  DOT,  DOT}  },
    [A_2_IDX('I')] = { .symbol = {DOT,  DOT}              },
    [A_2_IDX('J')] = { .symbol = {DOT,  DASH, DASH, DASH} },
    [A_2_IDX('K')] = { .symbol = {DASH, DOT,  DASH}       },
    [A_2_IDX('L')] = { .symbol = {DOT,  DASH, DOT,  DOT}  },
    [A_2_IDX('M')] = { .symbol = {DASH, DASH}             },
    [A_2_IDX('N')] = { .symbol = {DASH, DOT}              },
    [A_2_IDX('O')] = { .symbol = {DASH, DASH, DASH}       },
    [A_2_IDX('P')] = { .symbol = {DOT,  DASH, DASH, DOT}  },
    [A_2_IDX('Q')] = { .symbol = {DASH, DASH, DOT,  DASH} },
    [A_2_IDX('R')] = { .symbol = {DOT,  DASH, DOT}        },
    [A_2_IDX('S')] = { .symbol = {DOT,  DOT,  DOT}        },
    [A_2_IDX('T')] = { .symbol = {DASH}                   },
    [A_2_IDX('U')] = { .symbol = {DOT,  DOT,  DASH}       },
    [A_2_IDX('V')] = { .symbol = {DOT,  DOT,  DOT,  DASH} },
    [A_2_IDX('W')] = { .symbol = {DOT,  DASH, DASH}       },
    [A_2_IDX('X')] = { .symbol = {DASH, DOT,  DOT,  DASH} },
    [A_2_IDX('Y')] = { .symbol = {DASH, DOT,  DASH, DASH} },
    [A_2_IDX('Z')] = { .symbol = {DASH, DASH, DOT,  DOT}  },
};

/**
 * @brief Morse code arabic number lookup table
 */
const MorseChar_t MORSE_NUMERIC_TABLE[10] = {
    [N_2_IDX('0')] = { .symbol = {DASH, DASH, DASH, DASH, DASH} },
    [N_2_IDX('1')] = { .symbol = {DOT,  DASH, DASH, DASH, DASH} },
    [N_2_IDX('2')] = { .symbol = {DOT,  DOT,  DASH, DASH, DASH} },
    [N_2_IDX('3')] = { .symbol = {DOT,  DOT,  DOT,  DASH, DASH} },
    [N_2_IDX('4')] = { .symbol = {DOT,  DOT,  DOT,  DOT,  DASH} },
    [N_2_IDX('5')] = { .symbol = {DOT,  DOT,  DOT,  DOT,  DOT} },
    [N_2_IDX('6')] = { .symbol = {DASH, DOT,  DOT,  DOT,  DOT} },
    [N_2_IDX('7')] = { .symbol = {DASH, DASH, DOT,  DOT,  DOT} },
    [N_2_IDX('8')] = { .symbol = {DASH, DASH, DASH, DOT,  DOT} },
    [N_2_IDX('9')] = { .symbol = {DASH, DASH, DASH, DASH, DOT} },
};
