# Morse from C String

Now that you have some experience with morse code, it is time to turn that
knowledge into a re-usable module. For the purposes of this exercise, a module
is defined as a header (.h) and source (.c) pair that encapsulates a specific
piece of functionality. Below are some snippets that show the skeleton of 
a typical C module.

### Module Header (mod.h)
```c
#ifndef MOD_H
#define MOD_H

#ifdef __cplusplus
extern "C" {
#endif

void mod_init(void);     /* initialize internals               */
void mod_deinit(void);   /* de-initialize (usually not needed) */
void mod_task(void);     /* periodic background processing     */
void mod_process(int x); /* another API function               */

#ifdef __cplusplus
}
#endif

#endif /* MOD_H */

```

### Module Source (mod.c)
```c
#include "mod.h"

static int mod_global; /* global only visible to mod */

void mod_init(void)
{
    /* initialize statics, globals, call other init functions */
    mod_global = 0;
}

void mod_deinit(void)
{
    /* do any module de-init clean up or deletions */
}

void mod_task(void)
{
    /* call from main loop or scheduler (often called at a specific rate) */
    mod_global += 1;
}

void mod_process(int x)
{
    /* do something with module data or enqueue data from the mod_task */
    mod_global = x;
}
```

## Exercise

Refactor the morse code logic into a module (header and source) with an API
function that can convert any C-style (`const char *`) string into morse code.
You are free to set the maximum C string length (which will dictate the maximum
morse code symbols).

After the morse code module has been refactored, write an application that will
blink a message of your choosing then wait 3 seconds before transmitting again.
To make things easier, the 3 second delay starts after the final symbol 
including sentence, word, and character gaps.

### Bonus

Rewrite the 03_hello_morse_application using the new morse code API.

