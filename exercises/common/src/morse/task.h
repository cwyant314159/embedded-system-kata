#ifndef MORSE_TASK_H
#define MORSE_TASK_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void morse_task_init(void);
void morse_task(void);
void morse_task_encode(const char * c_str, bool_t repeat);
bool_t morse_task_is_encoding(void);
bool_t morse_task_is_repeat(void);

#ifdef __cplusplus
}
#endif

#endif /* MORSE_H */
