#include "bsp/bsp.h"
#include "morse/task.h"
#include "types.h"

/**
 * @brief Scheduler operating contexts.
 *
 * To support consistent timing of function calls, the scheduler has two
 * operating contexts within a given minor cycle.
 *
 * Time critical/important functions should be called from the PRIMARY context.
 * The PRIMARY context is executed once per minor cycle and is executed first
 * within the minor cycle. Upon completion of the PRIMARY context's execution,
 * the scheduler transitions to the BACKGROUND context. The BACKGROUND context
 * is executed repeatedly until the end of the minor cycle.
 */
typedef enum scheduler_context
{
    E_CONTEXT_PRIMARY,
    E_CONTEXT_BACKGROUND,
} SchedulerContext_t;

/*
 * The scheduler works on the concept of cycles. A (major) cycle is composed of
 * minor cycles. For this design, minor cycles last 100ms, and a major cycle is
 * composed of 10 minor cycles (1000ms).
 */
#define NUM_MINOR_CYCLES    (10u)
#define MINOR_CYCLE_MS      (100u)                              /* 100ms  */
#define MAJOR_CYCLE_MS      (NUM_MINOR_CYCLES * MINOR_CYCLE_MS) /* 1000ms */


static volatile SchedulerContext_t curr_context;
static volatile size_t curr_minor_cycle;


static void primary_context(void);
static void background_context(void);
static void initialize_scheduler(void);
static void scheduler_isr(void);

/**
 * @brief Hello, Morse!
 *
 * Extend the LED SOS exercise to blink out HELLO MORSE<sentence gap>
 */
int main(void)
{
    /* Initialize the hardware and software modules */
    bsp_init();              /* board support (e.g. the LED) */
    morse_task_init();       /* morse code processing task */
    initialize_scheduler();  /* application scheduler (starts timer) */

    /* Repeatedly output the morse code for this message */
    morse_task_encode("Hello, Morse!", E_TRUE);

    /* Scheduler loop */
    while (1) {
        /* Execute the context as specified by the scheduler */
        if (E_CONTEXT_PRIMARY == curr_context) {
            primary_context();
        } else {
            background_context();
        }
    }

    return 0; /* Satisfy compiler. Should never get here */
}


/**
 * @brief PRIMARY execution context.
 *
 * This function defines the PRIMARY execution context of the application. It is
 * executed once every MINOR_CYCLE_MS at the start of a minor cycle (or close to
 * it depending on where the BACKGROUND context is during the scheduler ISR).
 *
 * Task called within this function should not block or delay as this may throw
 * off the scheduling of other tasks.
 */
static void primary_context(void)
{
    /* Tasks that should be called every cycle should go here. */
    morse_task();
    /* Tasks that should be called once per major cycle should go here in an
       appropriate slot. It is best practice to not overload a particular
       slot as this may impact scheduling. 
       
       NOTE: The number of cases is application specific and will shrink and
             grow based on scheduler configuration (NUM_MINOR_CYCLES). */
    switch (curr_minor_cycle) {
        case 0:     break;
        case 1:     break;
        case 2:     break;
        case 3:     break;
        case 4:     break;
        case 5:     break;
        case 6:     break;
        case 7:     break;
        case 8:     break;
        case 9:     break;
        
        /* The default case should never be reached unless there is a
           programming error with the minor cycle handling in the
           scheduler's ISR callback. */
        default: {
            // bsp_error_trap();
            break;
        }    
    }

    /* PRIMARY context is over. Transition the context to BACKGROUND. */
    curr_context = E_CONTEXT_BACKGROUND;
}


/**
 * @brief BACKGROUND execution context.
 *
 * This function defines the BACKGROUND execution context of the application.
 * It is executed repeatedly for the time remaining in a minor cycle after the
 * PRIMARY context has finished. For simple applications with very limited IO,
 * this function will likely be empty.
 *
 * Task called within this function should not block or delay as this may throw
 * off the scheduling of other tasks.
 */
static void background_context(void)
{
    /* Tasks that should be called every cycle should go here. */

    /* Tasks that should be called once per major cycle should go here in an
       appropriate slot. It is best practice to not overload a particular
       slot as this may impact scheduling. 
       
       NOTE: The number of cases is application specific and will shrink and
             grow based on scheduler configuration (NUM_MINOR_CYCLES). */
    switch (curr_minor_cycle) {
        case 0:     break;
        case 1:     break;
        case 2:     break;
        case 3:     break;
        case 4:     break;
        case 5:     break;
        case 6:     break;
        case 7:     break;
        case 8:     break;
        case 9:     break;
        
        /* The default case should never be reached unless there is a
           programming error with the minor cycle handling in the
           scheduler's ISR callback. */
        default: {
            // bsp_error_trap();
            break;
        }    
    }
}


/**
 * @brief Scheduler initialization.
 *
 * The scheduler's initial state on power up is the BACKGROUND context in the
 * final minor cycle. This allows the first execution of minor cycle 0 to line
 * up exactly on a scheduler transition.
 *
 * The scheduler timer is also started which begins the countdown (or more
 * accurately count up) to the next transition.
 */
static void initialize_scheduler(void)
{
    curr_minor_cycle = NUM_MINOR_CYCLES - 1;    /* last cycle */
    curr_context     = E_CONTEXT_BACKGROUND;    /* in the background context */

    /* Enable interrupts before starting the timer so the first interrupt won't
       be missed. */
    bsp_enable_interrupts();

    /* ISR configuration */
    bsp_register_timer_isr_callback(scheduler_isr);
    bsp_set_timer_period_msec(MINOR_CYCLE_MS);
}


/**
 * @brief Scheduler ISR for context switching.
 *
 * This decrements the ticks remaining for the current morse symbol. Once the
 * tick count reaches zero, the array index is advanced for the next morse
 * element. When all the elements have been "toggled" out, the index wraps back
 * to the beginning.
 */
static void scheduler_isr(void)
{
    /* Transition the scheduler context back to the PRIMARY context. In a real
       system, it would be smart to check if the scheduler is still within the
       PRIMARY context. If context is still PRIMARY by the time this ISR runs,
       that would indicate a minor cycle overrun. */
    curr_context = E_CONTEXT_PRIMARY;

    /* Increment to the next minor cycle and roll back to zero when the cycle
       limit has been reached. */
    curr_minor_cycle += 1;
    if (NUM_MINOR_CYCLES <= curr_minor_cycle) {
        curr_minor_cycle = 0;
    }
}