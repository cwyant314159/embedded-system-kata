#ifndef BYTES_H
#define BYTES_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void bytes_set(uint8_t * const bytes, size_t len, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* BYTES_H */