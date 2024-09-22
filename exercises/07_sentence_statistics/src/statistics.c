#include "statistics.h"

#include "bsp/bsp.h"
#include "utils/ascii_char.h"
#include "utils/bytes.h"
#include "types.h"

/**
 * @brief A statistics meseaurement element.
 */
typedef struct element
{
    u8_t   count;
    bool_t clamped;
} Element_t;

/**
 * @brief Statistics Module Context
 *
 * The module context is made up of measurement elements of all the required
 * measurements listed in the exercise.md.
 */
typedef struct context
{
    Element_t letters;
    Element_t vowels;
    Element_t digits;
    Element_t whitespace;
    Element_t punctuation;
} Context_t;

static void reset_context(Context_t *p_ctx);
static void process_char(Context_t*p_ctx, char byte);
static void saturate_increment(Element_t *p_elem);
static void output_context(Context_t *p_ctx);
static void output_element(Element_t *p_elem);
static void num_to_c_str(u8_t num, char * c_str);
static void write_c_str(const char * const c_str);

static Context_t ctx;

void statistics_init(void)
{
    reset_context(&ctx);
}

void statistics_task(void)
{
    u8_t byte;

    if (E_TRUE == bsp_serial_read(&byte)) {
        bsp_serial_write(byte); /* Echo for easier typing */
        process_char(&ctx, (char)byte);
    }
}

static void reset_context(Context_t *p_ctx)
{
    /* This makes the assumption that false is the value 0. */
    bytes_set((u8_t*)p_ctx, sizeof(*p_ctx), 0x00);
}

static void process_char(Context_t*p_ctx, char c)
{
    if (E_TRUE == ascii_char_is_alpha(c)) {
        saturate_increment(&p_ctx->letters);

        if (E_TRUE == ascii_char_is_vowel(c)) {
            saturate_increment(&p_ctx->vowels);
        }
    } else if (E_TRUE == ascii_char_is_numeric(c)) {
        saturate_increment(&p_ctx->digits);
    } else if (E_TRUE == ascii_char_is_whitespace(c)) {
        saturate_increment(&p_ctx->whitespace);
    } else if (E_TRUE == ascii_char_is_punctuation(c)) {
        saturate_increment(&p_ctx->punctuation);
    }

    if ('\n' == c) {
        output_context(p_ctx);
        reset_context(p_ctx);
    }
}

static void saturate_increment(Element_t *p_elem)
{
    /* Increment if the count is already at the max value. */
    if (255 != p_elem->count) {
        p_elem->count += 1;
    }

    /* If the count is at max value, flag it as clamped. */
    if (255 == p_elem->count) {
        p_elem->clamped = E_TRUE;
    }
}

static void output_context(Context_t *p_ctx)
{
    write_c_str("\n=================\n");
    write_c_str("Letters    : ");
    output_element(&p_ctx->letters);
    write_c_str("\n");

    write_c_str("Vowels     : ");
    output_element(&p_ctx->vowels);
    write_c_str("\n");

    write_c_str("Digits     : ");
    output_element(&p_ctx->digits);
    write_c_str("\n");

    write_c_str("Whitespace : ");
    output_element(&p_ctx->whitespace);
    write_c_str("\n");

    write_c_str("Punctuation: ");
    output_element(&p_ctx->punctuation);
    write_c_str("\n\n");
}

static void output_element(Element_t *p_elem)
{
    static const char * const clamp_str = "+";

    /* Since the count is clamped to 255, we only need to handle 4 characters of
       data (1 for each digit plus NULL) */
    char c_str_buffer[4] = { 0 };

    /* convert from integer to c string */
    num_to_c_str(p_elem->count, c_str_buffer);

    /* Output the numerical data */
    write_c_str(c_str_buffer);
    if (p_elem->clamped) {
        write_c_str(clamp_str);
    }
}

static void num_to_c_str(u8_t num, char * c_str)
{
    size_t len;
    size_t i;
    u8_t   n;
    u8_t   digit;
    char   temp;

    /* Exit early if the number is 0. */
    if (0 == num) {
        c_str[0] = '0';
        c_str[1] = '\0';
    } else {
        /* This is a little inefficient since we go through two loops, but since the
           number is limited to 3 digits, this is ok... for now.

           This first loop parses the digits of num into characters and also
           computes the length (number of digits) of the string. The parsing
           puts the character representation of the digits in reverse order.

           NOTE: The len variable is a little overloaded in this context. It is
                 used as the index into the c_str and the length of the c_str.*/
        n   = num;
        len = 0;
        while(n != 0) {
            digit = n % 10;
            c_str[len] = ascii_char_digit_to_ascii(digit);
            len++;
            n /= 10;
        }

        /* Reverse the characters in the C string so the digits display
           correctly.

           NOTE: Only need to traverse half the array for reversal. */
        for (i = 0; i < len/2; i += 1) {
            temp = c_str[len - (i + 1)];
            c_str[len - (i + 1)] = c_str[i];
            c_str[i]             = temp;

        }

        /* Null terminate the string */
        c_str[len] = '\0';
    }
}

static void write_c_str(const char * const c_str)
{
    const char *curr;

    for (curr = c_str; *curr != '\0'; curr += 1) {
        if (E_FALSE == bsp_serial_write(*curr)) {
            bsp_error_trap();
        }
    }
}