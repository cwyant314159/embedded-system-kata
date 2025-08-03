#include "utils/bytes.h"

/**
 * @brief Set a byte array to a value
 *
 * @param[inout] bytes buffer to set
 * @param[in] len length of the input buffer
 * @param[in] value byte value to assign to all bytes of the buffer
 */
void bytes_set(uint8_t * const bytes, size_t len, uint8_t value)
{
    for (size_t i = 0; i < len; i += 1) {
        bytes[i] = value;
    }
}
