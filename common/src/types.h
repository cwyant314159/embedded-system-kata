/**
 * @brief Single header for all project specified data types.
 */
#ifndef TYPES_H
#define TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp/private/data_types/boolean_types.h"
#include "bsp/private/data_types/constants.h"
#include "bsp/private/data_types/int_types.h"

/*
 * ISR callback function pointer type.
 *
 * Maps to function with the signature void my_callback(void)
 */
typedef void (*IsrCallback_t)(void);


#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */