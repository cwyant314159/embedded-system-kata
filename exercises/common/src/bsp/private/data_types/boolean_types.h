/**
 * @brief Boolean type with common (more readable) aliases.
 */
#ifndef BOOLEAN_H
#define BOOLEAN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum bool_type
{
    E_FALSE = 0,
    E_TRUE  = 1,
} bool_t;

typedef enum pass_fail_type
{
    E_PASS = E_TRUE,
    E_FAIL = E_FALSE,
} pass_t;

typedef enum on_off_type
{
    E_ON  = E_TRUE,
    E_OFF = E_FALSE,
} on_off_t;

typedef enum high_low_type
{
    E_HIGH = E_TRUE,
    E_LOW  = E_FALSE,
} hi_lo_t;

typedef enum bit_type
{
    E_BIT_1 = E_TRUE,
    E_BIT_0 = E_FALSE,
} bit_t;

typedef enum input_output_t
{
    E_OUTPUT = E_TRUE,
    E_INPUT  = E_FALSE,
} io_t;

#ifdef __cplusplus
}
#endif

#endif /* BOOLEAN_H */