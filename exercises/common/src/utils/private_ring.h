/**
 * @file private_ring.h
 * @brief Generic ring buffer implementation that is meant to be private to a
 * C module
 *
 *
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#ifndef PRIVATE_RING_H
#define PRIVATE_RING_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PRIVATE_RING_SIZE
    #define PRIVATE_RING_SIZE  (16u)
#endif

#if ((PRIVATE_RING_SIZE & (PRIVATE_RING_SIZE-1)) != 0)
    #error PRIVATE_RING_SIZE must be a power of 2!
#endif

#ifdef PRIVATE_RING_VOLATILE_DECL
    #define PRIVATE_RING_VOLATILE__ volatile
#else
    #define PRIVATE_RING_VOLATILE__
#endif

#define PRIVATE_RING_DECLARATIONS(T_RING, T)                                            \
typedef struct T_RING ## pointer_ring                                                   \
{                                                                                       \
    size_t head;                                                                        \
    size_t tail;                                                                        \
    T data[PRIVATE_RING_SIZE];                                                          \
} T_RING ## PrivateRing_t;                                                              \
                                                                                        \
static inline void T_RING ## prv_ring_init(PRIVATE_RING_VOLATILE__ T_RING ## PrivateRing_t *p_ring) \
{                                                                                       \
    p_ring->head  = 0;                                                                  \
    p_ring->tail = PRIVATE_RING_SIZE - 1;                                               \
                                                                                        \
    for (size_t i = 0; i < PRIVATE_RING_SIZE; i += 1) {                                 \
        p_ring->data[i] = 0;                                                            \
    }                                                                                   \
}                                                                                       \
                                                                                        \
static inline bool_t T_RING ## prv_ring_is_empty(PRIVATE_RING_VOLATILE__ T_RING ## PrivateRing_t *p_ring) \
{                                                                                       \
    size_t next_tail = (p_ring->tail + 1) & (PRIVATE_RING_SIZE - 1);                    \
    return next_tail == p_ring->head;                                                   \
}                                                                                       \
                                                                                        \
static inline bool_t T_RING ## prv_ring_is_full(PRIVATE_RING_VOLATILE__ T_RING ## PrivateRing_t *p_ring) \
{                                                                                       \
    size_t next_head = (p_ring->head + 1) & (PRIVATE_RING_SIZE - 1);                    \
    return next_head == p_ring->tail;                                                   \
}                                                                                       \
                                                                                        \
static inline void T_RING ## prv_ring_push(PRIVATE_RING_VOLATILE__ T_RING ## PrivateRing_t *p_ring, T d) \
{                                                                                       \
    if (E_FALSE == T_RING ## prv_ring_is_full(p_ring)) {                                \
        p_ring->data[p_ring->head] = d;                                                 \
        p_ring->head = (p_ring->head + 1) & (PRIVATE_RING_SIZE - 1);                    \
                                                                                        \
    }                                                                                   \
}                                                                                       \
                                                                                        \
static inline T T_RING ## prv_ring_pop(PRIVATE_RING_VOLATILE__ T_RING ## PrivateRing_t *p_ring) \
{                                                                                       \
    size_t next_tail = (p_ring->tail + 1) & (PRIVATE_RING_SIZE - 1);                    \
    T d = p_ring->data[next_tail];                                                      \
                                                                                        \
    if (E_FALSE ==  T_RING ## prv_ring_is_empty(p_ring)) {                              \
        p_ring->data[p_ring->head] = d;                                                 \
        p_ring->tail = next_tail;                                                       \
    }                                                                                   \
                                                                                        \
    return d;                                                                           \
}                                                                                       \
                                                                                        \
static inline T T_RING ## prv_ring_peek(PRIVATE_RING_VOLATILE__ T_RING ## PrivateRing_t *p_ring) \
{                                                                                       \
    size_t next_tail = (p_ring->tail + 1) & (PRIVATE_RING_SIZE - 1);                    \
    T d = p_ring->data[next_tail];                                                      \
    return d;                                                                           \
}

#define PRIVATE_RING_DECLARE(T_RING, var_name)      T_RING ## PrivateRing_t var_name

#define PRIVATE_RING_INIT(T_RING, var_name)         T_RING ## prv_ring_init(&var_name)
#define PRIVATE_RING_IS_EMPTY(T_RING, var_name)     T_RING ## prv_ring_is_empty(&var_name)
#define PRIVATE_RING_IS_FULL(T_RING, var_name)      T_RING ## prv_ring_is_full(&var_name)
#define PRIVATE_RING_PUSH(T_RING, var_name, data)   T_RING ## prv_ring_push(&var_name, data)
#define PRIVATE_RING_POP(T_RING, var_name)          T_RING ## prv_ring_pop(&var_name)
#define PRIVATE_RING_PEEK(T_RING, var_name)         T_RING ## prv_ring_peek(&var_name)

#ifdef __cplusplus
}
#endif

#endif /* PRIVATE_RING_H */