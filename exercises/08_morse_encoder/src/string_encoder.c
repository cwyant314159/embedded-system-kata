#include "string_encoder.h"

#include "bsp/bsp.h"
#include "morse/task.h"
#include "utils/bytes.h"

#define MAX_STRING_LEN 41 /* +1 to handle null terminator */

static const char* ERROR_STRING = "\n\rERROR: Encoding already in progress!\n\r";

static size_t the_string_idx;
static u8_t   the_string[MAX_STRING_LEN];

static void module_reset(void);
static void handle_newline(void);
static void handle_morse_byte(u8_t byte);

/**
 * @brief Initialize the string encoder and its internal data.
 */
void string_encoder_init(void)
{
    module_reset();
}

/**
 * @brief String encoder process
 * 
 * NOTE: This function should be called as often as possible to prevent serial
 *       port data loss.
 */
void string_encoder_process(void)
{
    u8_t rx_char;

    if (E_TRUE == bsp_serial_read(&rx_char)) {
        switch(rx_char)
        {
            case '\0' :
            case '~'  :
            case '`'  :
            case '@'  :
            case '#'  :
            case '$'  :
            case '%'  :
            case '^'  :
            case '&'  :
            case '*'  :
            case '('  :
            case ')'  :
            case '-'  :
            case '_'  :
            case '='  :
            case '+'  :
            case '['  :
            case '{'  :
            case ']'  :
            case '}'  :
            case '\\' :
            case '|'  :
            case ';'  :
            case ':'  :
            case '\'' :
            case '"'  :
            case ','  :
            case '<'  :
            case '/'  :
            case '\r' : /* DO NOTHING*/             break; /* Ignore these characters */

            case '\n': handle_newline();            break; /* Sentence terminator */
            default:   handle_morse_byte(rx_char);  break; /* Characters we can encode. */
        }
    }
}

/**
 * @brief Reset the internals of the string encoder.
 */
static void module_reset(void)
{
    the_string_idx = 0;
    bytes_set(the_string, sizeof(the_string), 0x00);
}

/**
 * @brief Enqueue a byte that is morse encode-able into the string buffer.
 * 
 * @param[in] byte morse encode-able byte
 */
static void handle_morse_byte(u8_t byte)
{
    if (the_string_idx < (MAX_STRING_LEN-1)) {
        bsp_serial_write(byte);             /* echo */
        the_string[the_string_idx] = byte;  /* add to our string */
        the_string_idx += 1;                /* move to the next element */
    }
}

/**
 * @brief Handle the new line character
 * 
 * The new line character is our string terminator. One of two thing will happen
 * when a new line is received:
 * 
 * 1. If the morse code module is already encoding a string, an error will be
 *    transmitted over the serial port.
 * 
 * 2. If the morse code module is idle, the buffered string will be sent to the
 *    morse module for encoding.
 * 
 * In both cases, the module internals a reset for the next string.
 */
static void handle_newline(void)
{
    /* If the morse code task is already encoding a string, we can't encode
       another string. Transmit the error message. Otherwise, the morse code
       task is waiting on us to give it a new string to encode.
       
       Remember to NULL terminate the string for the morse code task!! */
    if (E_TRUE == morse_task_is_encoding()) {
        bsp_serial_write_c_str(ERROR_STRING);
    } else {
        morse_task_encode((char*)the_string, E_FALSE);
    }

    /* This moves the user's cursor down a line on their terminal */
    bsp_serial_write_c_str("\n\r");

    /* Always reset after getting a new line to prepare for the next string. */
    module_reset();
}
