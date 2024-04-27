#include "bsp/crt0.h"

#include <avr/pgmspace.h>

#include "types.h"

extern u16_t ld__data_begin;        /* RAM DATA section first entry */
extern u16_t ld__data_end;          /* RAM DATA section one past last entry */
extern u16_t ld__data_load_begin;   /* flash DATA section first entry */

extern u16_t ld__bss_begin;         /* BSS section first entry */
extern u16_t ld__bss_end;           /* BSS section one past last entry */

extern u16_t ld__ctors_begin;       /* C++ ctor first entry */
extern u16_t ld__ctors_end;         /* C++ ctor one past last entry */
typedef void (*fpCtor)(void);       /* pointer to C++ constructor */

extern int main(void);              /* the main function */

/**
 * @brief Startup C routine
 * 
 * This routine is called from the assembly startup code and expects that prior 
 * to being called the R1 register is 0, the SREG has been cleared, and that the
 * stack pointer has been configured.
 * 
 * The startup C routine is responsible for copying data from flash to the DATA
 * section of RAM, clearing (zeroing) the BSS section, and calling global/static
 * C++ constructors.
 * 
 * Once the RAM and constructors are initialized this routine will call the main
 * function. If the main function returns, execution is trapped in an infinite
 * loop.
 */
void startup_c__(void)
{
    /*
     * Iterate over the DATA section in RAM and copy the data from flash.
     *
     * Since the actual content of the DATA section resides in flash, the
     * pgm_read_x family of functions is needed to actually read the data from
     * flash memory. A simple dereference of the pointer does not produce the
     * proper instructions for flash reading.
     */
    u8_t* data_begin      = (u8_t*)&ld__data_begin;
    u8_t* data_end        = (u8_t*)&ld__data_end;
    const u8_t* load_data = (u8_t*)&ld__data_load_begin;

    for (u8_t* data_ptr = data_begin; data_ptr != data_end; data_ptr += 1) {
        *data_ptr = pgm_read_byte(load_data++);
    }

    /*
     * Iterate over the BSS section and write 0's.
     *
     * The BSS section resides entirely in RAM and can be accessed by just using
     * pointer dereferencing.
     */
    u8_t* bss_begin = (u8_t*)&ld__bss_begin;
    u8_t* bss_end   = (u8_t*)&ld__bss_end;

    for (u8_t* bss_ptr = bss_begin; bss_ptr != bss_end; bss_ptr += 1) {
        *bss_ptr = 0u;
    }

    /*
     * Iterate over the C++ constructors (in flash) and call them.
     *
     * The values iterated over are the addresses of the constructors. To call
     * them, the address must be read from flash with the pgm_read_word function
     * (remember pointers are 16-bit and a word is 16 bits).
     */
    u16_t* ctor_begin = (u16_t*)&ld__ctors_begin;
    u16_t* ctor_end   = (u16_t*)&ld__ctors_end;
    fpCtor ctor;
    for (u16_t* c = ctor_begin; c != ctor_end; c += 1) {
        ctor = (fpCtor)pgm_read_word(c);
        ctor();
    }

    /*
     * Call the applications main function.
     */
    main();

    /*
     * Trap execution if main ever returns.
     */
    while(1) {  }
}