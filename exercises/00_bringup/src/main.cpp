#include "bsp/bsp.h"
#include "types.h"


static pass_t startup_check(void);


/*
 * Verification of the BSS segment is fairly straightforward. Any global
 * variable that is not initialized to a value is set to 0 by the startup
 * routine. The verification routine simply checks that the variables and
 * array below are 0.
 */
#define BSS_ARRAY_SIZE (16)

unsigned char  bss0;
unsigned short bss1;
unsigned long  bss2;
unsigned char  bss_array[BSS_ARRAY_SIZE];


/*
 * Verification of the DATA segment ensures that any global variable that is
 * initialized to a value is the set to the appropriate value. The verification
 * routine compare the vairables (data0..2) and the array to the #define'ed
 * constants EXPECTED_D0..2. 
 * 
 * NOTE: It is important that macros are used and not static const variables.
 *       The mechanism to set the static const variables is under test.
 */
#define EXPECTED_D0 (42u)
#define EXPECTED_D1 (65000u)
#define EXPECTED_D2 (4000000000lu)
#define DATA_ARRAY_SIZE (3)

unsigned char  data0 = EXPECTED_D0;
unsigned short data1 = EXPECTED_D1;
unsigned long  data2 = EXPECTED_D2;
unsigned long  data_array[DATA_ARRAY_SIZE] = {
    EXPECTED_D0,
    EXPECTED_D1,
    EXPECTED_D2
};

/*
 * The global C++ constructor test will verify the following:
 *      - global constructor execution by incrementing the g_ctor_count a
 *        NUM_GLOBAL_OBJS number of times
 *
 *      - static member variables by incrementing the static variable a
 *        NUM_GLOBAL_OBJS number of times
 *
 *      - static constant members by verifying the constant value in all objects
 */
#define EXPECTED_MEM_CONST  (0xAFu)
#define NUM_GLOBAL_OBJS     (3u)

unsigned char g_ctor_count;

struct FooBar {
    FooBar()
    {
        g_ctor_count += 1;
        memVar       += 1;
    }

    ~FooBar() = default;

    static unsigned char memVar;
    static const unsigned char memConst = EXPECTED_MEM_CONST;
};

unsigned char FooBar::memVar = 0u;

FooBar global_objs[NUM_GLOBAL_OBJS];

/**
 * @brief Startup routine verification application
 * 
 * RAM segments and C++ global object behavior is inspected for proper handling
 * by the startup code (crt0.s and crt0.c). If all the checks have passed, the
 * builtin LED will blink. The LED will not blink if any of the checks fail.
 */
int main(void)
{
    bsp_init();

    while (1) {
        if (E_PASS == startup_check()) {
            bsp_toggle_builtin_led();
        }

        bsp_spin_delay(5u);
    }

    return 0; /* Satisfy compiler. Should never get here */
}

/**
 * @brief Verify startup routine properly executed
 * 
 * This function verifies the following:
 *  - uninitialized global variables (and arrays) are 0
 *  - initialized global variables (and arrays) are expected values
 *  - all global C++ constructors have been called
 *  - global C++ object static members are expected values (from class)
 *  - global C++ object static members are epxected values (per object)
 *  - global C++ object static constants are epxected values (from class)
 *  - global C++ object static constants are epxected values (from object)
 * 
 * @return E_TRUE - all checks passed
 * @return E_FALSE - any of the above mentioned checks failed.
 */
static pass_t startup_check(void)
{
    pass_t result;  /* check pass/fail result */
    size_t i;       /* loop counter */

    /* Assume the test will passes... until it doesn't */
    result = E_PASS;

    /* Use a do-while loop to emulate multiple returns by utilizing break. */
    do {
        /* BSS segment variable test */
        if (0 != bss0 || 0 != bss1 || 0 != bss2) {
            result = E_FAIL;
            break;
        }

        /* BSS segment array test */
        for (i = 0; i < BSS_ARRAY_SIZE; i += 1) {
            if (bss_array[i] != 0) {
                result = E_FAIL;
                break;
            }
        }

        /* data segment variable test */
        if (EXPECTED_D0 != data0 || EXPECTED_D1 != data1 || EXPECTED_D2 != data2) {
            result = E_FAIL;
            break;
        }

        /* data segment variable test */
        for (i = 0; i < DATA_ARRAY_SIZE; i += 1) {
            if ( ( (0 == i) && (EXPECTED_D0 != data_array[i]) ) ||
                 ( (1 == i) && (EXPECTED_D1 != data_array[i]) ) ||
                 ( (2 == i) && (EXPECTED_D2 != data_array[i]) )  ) { 
                result = E_FAIL;
                break;
            }
        }

        /* global C++ constructor check */
        if (NUM_GLOBAL_OBJS != g_ctor_count) {
            result = E_FAIL;
            break;
        }

        /* global static member check (class access) */
        if (NUM_GLOBAL_OBJS != FooBar::memVar) {
            result = E_FAIL;
            break;
        }

        /* global static member check (per object access) */
        for (i = 0; i < NUM_GLOBAL_OBJS; i += 1) {
            if (NUM_GLOBAL_OBJS != global_objs[i].memVar) {
                result = E_FAIL;
                break;
            }
        }

        /* global static constant check (class access) */
        if (EXPECTED_MEM_CONST != FooBar::memConst) {
            result = E_FAIL;
            break;
        } 

        for (i = 0; i < NUM_GLOBAL_OBJS; i += 1) {
            if (EXPECTED_MEM_CONST != global_objs[i].memConst) {
                result = E_FAIL;
                break;
            }
        }

    } while ( 0 );

    return result;
}
