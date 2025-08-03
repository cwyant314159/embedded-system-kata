#ifndef MORSE_TASK_H
#define MORSE_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#define MORSE_TASK_ENABLE_REPEAT  1
#define MORSE_TASK_DISABLE_REPEAT 0

void morse_task_init(void);
void morse_task(void);
void morse_task_encode(const char * c_str, int repeat);
int morse_task_is_encoding(void);
int morse_task_is_repeat(void);

#ifdef __cplusplus
}
#endif

#endif /* MORSE_H */
