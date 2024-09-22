#ifndef BYTES_H
#define BYTES_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void bytes_set(u8_t * const bytes, size_t len, u8_t value);

#ifdef __cplusplus
}
#endif

#endif /* BYTES_H */