/**
 * @brief Fixed width integer typedefs.
 */
#ifndef INT_TYPES_H
#define INT_TYPES_H

typedef float FLOAT_T;

typedef signed   int ssize_t;
typedef unsigned int size_t;

typedef signed char         s8_t;
typedef signed short        s16_t;
typedef signed long         s32_t;
typedef signed long long    s64_t;

typedef unsigned char       u8_t;
typedef unsigned short      u16_t;
typedef unsigned long       u32_t;
typedef unsigned long long  u64_t;

/* 
 * When compiling with C11, use static_asserts to verify the widths of the types
 * above.
 * 
 * The defined() check is added to suppress compiler warnings that incorrectly
 * assum __STDC_VERSION__ is not defined.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #include <assert.h>

    static_assert(sizeof(s8_t)  == 1, "fixed width integer");
    static_assert(sizeof(s16_t) == 2, "fixed width integer");
    static_assert(sizeof(s32_t) == 4, "fixed width integer");
    static_assert(sizeof(s64_t) == 8, "fixed width integer");

    static_assert(sizeof(u8_t)  == 1, "fixed width integer");
    static_assert(sizeof(u16_t) == 2, "fixed width integer");
    static_assert(sizeof(u32_t) == 4, "fixed width integer");
    static_assert(sizeof(u64_t) == 8, "fixed width integer");
#endif

#endif /* INT_TYPES_H */