#ifndef BIG_INT_H
#define BIG_INT_H

#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

#ifndef BOOL
#define BOOL int8_t
#endif

typedef struct BigInt {
    unsigned char* digits; // Array of digits 0-9.  Greater indices hold more significant digits.
    unsigned int num_digits; // Number of digits actually in the number.
    unsigned int num_allocated_digits; // digits array has space for this many digits
    BOOL is_negative; // Nonzero if this BigInt is negative, zero otherwise.
} BigInt;

//============================================================================
// Construction and assignment
//============================================================================

// Returns a pointer to a new BigInt initialized to the specified value.
// Caller is responsible for freeing the new BigInt with a
// corresponding call to BigInt_free.
// returns NULL on memory allocation failure.
BigInt* BigInt_construct(int value);

// Returns a pointer to a new BigInt initialized from the supplied BigInt.
// Caller is responsible for freeing the new BigInt with a
// corresponding call to BigInt_free.
// returns NULL on memory allocation failure.
BigInt* BigInt_clone(const BigInt* big_int, unsigned int num_allocated_digits);

// Returns a pointer to a new BigInt initialized from digits in the specified
// zero-terminated string. Caller is responsible for freeing the new BigInt
// with a corresponding call to BigInt_free
BigInt* BigInt_from_string(const char* str);

// Frees the memory for a BigInt allocated using BigInt_construct.
void BigInt_free(BigInt* big_int);

///Sets the value of the target BigInt to the value of the source BigInt.
// Assumes that target and source already point to valid BigInts.
// returns non-zero on success or 0 on failure
BOOL BigInt_assign(BigInt* target, const BigInt* source);

///Sets the value of the target BigInt to the value of the source int.
// returns non-zero on success or 0 on failure
BOOL BigInt_assign_int(BigInt* target, const int source);

// Prints the contents of big_int to stdout.
void BigInt_print(const BigInt* big_int);

// Prints the contents of big_int to output stream.
void BigInt_fprint(FILE *dest, const BigInt* big_int);

// what would be the length of a string if this BigInt were converted to a string
// (see BigInt_to_string() below)
unsigned int BigInt_strlen(const BigInt* big_int);

// write BigInt to a string buffer, returns non-zero on success
// returns zero if BigInt doesn't fit into buf.
// buf_size *must* include the terminating zero byte
BOOL BigInt_to_string(const BigInt* big_int, char* buf, unsigned int buf_size);

// convert BigInt to a newly allocated string.
// returns NULL on failure.
char* BigInt_to_new_string(const BigInt* big_int);

//============================================================================
// Basic mathematical operations
//============================================================================

// Returns -1 if a < b, 0 if a == b, 1 if a > b 
int BigInt_compare(const BigInt* a, const BigInt* b);

// Returns -1 if a < b, 0 if a == b, 1 if a > b
int BigInt_compare_int(const BigInt* a, int b);

// Adds the value in addend to big_int.  Places the result in big_int.
// returns non-zero on success or 0 on failure
BOOL BigInt_add(BigInt* big_int, const BigInt* addend);
BOOL BigInt_add_int(BigInt* big_int, const int addend);

// Subtracts the value of to_subtract from big_int.
// Places the result in big_int.
// returns non-zero on success or 0 on failure
BOOL BigInt_subtract(BigInt* big_int, const BigInt* to_subtract);
BOOL BigInt_subtract_int(BigInt* big_int, const int to_subtract);

// Multiplies the value in big_int by multiplier.  Places the
// result in big_int.
// returns non-zero on success or 0 on failure
BOOL BigInt_multiply(BigInt* big_int, const BigInt* multiplier);
BOOL BigInt_multiply_int(BigInt* big_int, const int multiplier);

// Divides dividend by divisor.
// If only quotient is desired, remainder can be NULL
// If only remainder is desired, quotient can be NULL
// if both quotient and remainder are NULL, this is just a fancy way of burning cpu
// returns non-zero on success or 0 on failure
BOOL BigInt_divide(
    BigInt* dividend, BigInt* divisor,
    BigInt* quotient, BigInt* remainder
);

// Sets result to the value of big_int as an integer if the
// value of big_int fits within the size of result's type on the target
// environment.  returns non-zero on success or 0 on failure.
// result is undefined on failure.
BOOL BigInt_to_int(const BigInt* big_int, int* result);

//============================================================================
// Internal helpers
//============================================================================

// Ensure that big_int has space allocated for at least digits_needed digits.
// returns non-zero on success or 0 on failure
BOOL BigInt_ensure_digits(BigInt* big_int, unsigned int digits_needed);

// Performs an unsigned comparison of the two BigInt parameters; that is, the
// comparison is of their absolute values.  Returns 1 if |a| > |b|, 0 if |a| == |b|,
// and -1 if |a| < |b|.
int BigInt_compare_digits(const BigInt* a, const BigInt* b);

// Performs an unsigned addition of to_add to big_int; adds the digits without regard
// for the sign of either parameter. 
// returns non-zero on success or 0 on failure
BOOL BigInt_add_digits(BigInt* big_int, const BigInt* to_add);

// Performs an unsigned subtraction of to_subtract from big_int; subtracts the digits
// without regard for the sign of either parameter. 
// returns non-zero on success or 0 on failure
BOOL BigInt_subtract_digits(BigInt* big_int, const BigInt* to_subtract);

#endif // BIG_INT_H



#include <assert.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BIG_INT_H
#define BIG_INT_H

#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

#ifndef BOOL
#define BOOL int8_t
#endif

typedef struct BigInt {
    unsigned char* digits; // Array of digits 0-9.  Greater indices hold more significant digits.
    unsigned int num_digits; // Number of digits actually in the number.
    unsigned int num_allocated_digits; // digits array has space for this many digits
    BOOL is_negative; // Nonzero if this BigInt is negative, zero otherwise.
} BigInt;

//============================================================================
// Construction and assignment
//============================================================================

// Returns a pointer to a new BigInt initialized to the specified value.
// Caller is responsible for freeing the new BigInt with a
// corresponding call to BigInt_free.
// returns NULL on memory allocation failure.
BigInt* BigInt_construct(int value);

// Returns a pointer to a new BigInt initialized from the supplied BigInt.
// Caller is responsible for freeing the new BigInt with a
// corresponding call to BigInt_free.
// returns NULL on memory allocation failure.
BigInt* BigInt_clone(const BigInt* big_int, unsigned int num_allocated_digits);

// Returns a pointer to a new BigInt initialized from digits in the specified
// zero-terminated string. Caller is responsible for freeing the new BigInt
// with a corresponding call to BigInt_free
BigInt* BigInt_from_string(const char* str);

// Frees the memory for a BigInt allocated using BigInt_construct.
void BigInt_free(BigInt* big_int);

///Sets the value of the target BigInt to the value of the source BigInt.
// Assumes that target and source already point to valid BigInts.
// returns non-zero on success or 0 on failure
BOOL BigInt_assign(BigInt* target, const BigInt* source);

///Sets the value of the target BigInt to the value of the source int.
// returns non-zero on success or 0 on failure
BOOL BigInt_assign_int(BigInt* target, const int source);

// Prints the contents of big_int to stdout.
void BigInt_print(const BigInt* big_int);

// Prints the contents of big_int to output stream.
void BigInt_fprint(FILE *dest, const BigInt* big_int);

// what would be the length of a string if this BigInt were converted to a string
// (see BigInt_to_string() below)
unsigned int BigInt_strlen(const BigInt* big_int);

// write BigInt to a string buffer, returns non-zero on success
// returns zero if BigInt doesn't fit into buf.
// buf_size *must* include the terminating zero byte
BOOL BigInt_to_string(const BigInt* big_int, char* buf, unsigned int buf_size);

// convert BigInt to a newly allocated string.
// returns NULL on failure.
char* BigInt_to_new_string(const BigInt* big_int);

//============================================================================
// Basic mathematical operations
//============================================================================

// Returns -1 if a < b, 0 if a == b, 1 if a > b 
int BigInt_compare(const BigInt* a, const BigInt* b);

// Returns -1 if a < b, 0 if a == b, 1 if a > b
int BigInt_compare_int(const BigInt* a, int b);

// Adds the value in addend to big_int.  Places the result in big_int.
// returns non-zero on success or 0 on failure
BOOL BigInt_add(BigInt* big_int, const BigInt* addend);
BOOL BigInt_add_int(BigInt* big_int, const int addend);

// Subtracts the value of to_subtract from big_int.
// Places the result in big_int.
// returns non-zero on success or 0 on failure
BOOL BigInt_subtract(BigInt* big_int, const BigInt* to_subtract);
BOOL BigInt_subtract_int(BigInt* big_int, const int to_subtract);

// Multiplies the value in big_int by multiplier.  Places the
// result in big_int.
// returns non-zero on success or 0 on failure
BOOL BigInt_multiply(BigInt* big_int, const BigInt* multiplier);
BOOL BigInt_multiply_int(BigInt* big_int, const int multiplier);

// Divides dividend by divisor.
// If only quotient is desired, remainder can be NULL
// If only remainder is desired, quotient can be NULL
// if both quotient and remainder are NULL, this is just a fancy way of burning cpu
// returns non-zero on success or 0 on failure
BOOL BigInt_divide(
    BigInt* dividend, BigInt* divisor,
    BigInt* quotient, BigInt* remainder
);

// Sets result to the value of big_int as an integer if the
// value of big_int fits within the size of result's type on the target
// environment.  returns non-zero on success or 0 on failure.
// result is undefined on failure.
BOOL BigInt_to_int(const BigInt* big_int, int* result);

//============================================================================
// Internal helpers
//============================================================================

// Ensure that big_int has space allocated for at least digits_needed digits.
// returns non-zero on success or 0 on failure
BOOL BigInt_ensure_digits(BigInt* big_int, unsigned int digits_needed);

// Performs an unsigned comparison of the two BigInt parameters; that is, the
// comparison is of their absolute values.  Returns 1 if |a| > |b|, 0 if |a| == |b|,
// and -1 if |a| < |b|.
int BigInt_compare_digits(const BigInt* a, const BigInt* b);

// Performs an unsigned addition of to_add to big_int; adds the digits without regard
// for the sign of either parameter. 
// returns non-zero on success or 0 on failure
BOOL BigInt_add_digits(BigInt* big_int, const BigInt* to_add);

// Performs an unsigned subtraction of to_subtract from big_int; subtracts the digits
// without regard for the sign of either parameter. 
// returns non-zero on success or 0 on failure
BOOL BigInt_subtract_digits(BigInt* big_int, const BigInt* to_subtract);

#endif // BIG_INT_H



#ifndef C_SAFE_MATH_IMPL
#define C_SAFE_MATH_IMPL

#if defined _MSC_VER
// static inline expansion warnings
#pragma warning(disable:4710 4711)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    // It is a bit tricky to sort out what compiler we are actually using,
    // do this once here, and avoid cluttering the code
#define VISUAL_STUDIO_COMPILER 0
#define CLANG_COMPILER 1
#define GCC_COMPILER 2
#define UNKNOWN_COMPILER -1

// Clang will sometimes pretend to be Visual Studio
// and does pretend to be gcc. Check it first, as nothing else pretends to be clang
#if defined __clang__
#define SAFEINT_COMPILER CLANG_COMPILER
#elif defined __GNUC__
#define SAFEINT_COMPILER GCC_COMPILER
#elif defined _MSC_VER
#define SAFEINT_COMPILER VISUAL_STUDIO_COMPILER
#else
#define SAFEINT_COMPILER UNKNOWN_COMPILER
#endif

// Various defines to help make working with multiple compilers easier - from SafeInt.hpp
#if SAFEINT_COMPILER == GCC_COMPILER || SAFEINT_COMPILER == CLANG_COMPILER
#define SAFEINT_NORETURN __attribute__((noreturn))
#define SAFEINT_STDCALL
#define SAFEINT_VISIBLE __attribute__ ((__visibility__("default")))
#define SAFEINT_WEAK __attribute__ ((weak))
#else
#define SAFEINT_NORETURN __declspec(noreturn)
#define SAFEINT_STDCALL __stdcall
#define SAFEINT_VISIBLE
#define SAFEINT_WEAK
#endif

#if SAFEINT_COMPILER == VISUAL_STUDIO_COMPILER
    // limits.h checks __STDC_WANT_SECURE_LIB__, but doesn't include what sets it
#if !defined __STDC_WANT_SECURE_LIB__
#define __STDC_WANT_SECURE_LIB__ 0
#endif

#endif

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

// Figure out if we should use intrinsics
// If the user has already decided, let that override
#define SAFEINT_MULTIPLY_MATH        0 // no intrinsics, no built in, no 128-bit
#define SAFEINT_MULTIPLY_INTRINSICS  1 // 64-bit Visual Studio
#define SAFEINT_MULTIPLY_BUILTIN     2 // gcc, clang
#define SAFEINT_MULTIPLY_INT128      3 // Best case

// We might have 128-bit int support, check for that, as it should work best
#if !defined SAFEINT_HAS_INT128

#if defined __SIZEOF_INT128__ && __SIZEOF_INT128__ == 16
#define SAFEINT_HAS_INT128 1
#else
#define SAFEINT_HAS_INT128 0
#endif

#endif

#if SAFEINT_HAS_INT128
#define SAFEINT_MULTIPLY_METHOD SAFEINT_MULTIPLY_INT128
#else

#if !defined SAFEINT_USE_INTRINSICS
// If it is the Visual Studio compiler, then it has to be 64-bit, and not ARM64EC
#if SAFEINT_COMPILER == VISUAL_STUDIO_COMPILER
#if defined _M_AMD64 && !defined _M_ARM64EC
#include <intrin.h>
#define SAFEINT_MULTIPLY_METHOD SAFEINT_MULTIPLY_INTRINSICS
#else
#define SAFEINT_MULTIPLY_METHOD SAFEINT_MULTIPLY_MATH
#endif

#else // Not VISUAL_STUDIO_COMPILER

    // Else for gcc and clang, we can use builtin functions
#if SAFEINT_COMPILER == CLANG_COMPILER || SAFEINT_COMPILER == GCC_COMPILER
#define SAFEINT_MULTIPLY_METHOD SAFEINT_MULTIPLY_BUILTIN
#else
#define SAFEINT_MULTIPLY_METHOD SAFEINT_MULTIPLY_MATH
#endif
#endif

#endif // SAFEINT_USE_INTRINSICS
#endif // SAFEINT_HAS_INT128

/*
    To replace safe_math_fail, wrap this header,
    implement safe_math_fail how you prefer,
    and set SAFE_MATH_FAIL_DEFINED
*/

#if !defined SAFE_MATH_FAIL_DEFINED
#define SAFE_MATH_FAIL_DEFINED
#include <stdlib.h>

SAFEINT_NORETURN
static inline void safe_math_fail(const char* msg)
{
    (void)msg;
    abort();
}
#endif

#if !defined UINT64_MAX

#define INT8_MIN         (-127i8 - 1)
#define INT16_MIN        (-32767i16 - 1)
#define INT32_MIN        (-2147483647i32 - 1)
#define INT64_MIN        (-9223372036854775807i64 - 1)
#define INT8_MAX         127i8
#define INT16_MAX        32767i16
#define INT32_MAX        2147483647i32
#define INT64_MAX        9223372036854775807i64
#define UINT8_MAX        0xffui8
#define UINT16_MAX       0xffffui16
#define UINT32_MAX       0xffffffffui32
#define UINT64_MAX       0xffffffffffffffffui64

#endif

// Utility functions

// Purpose of this is to negate an int in a way
// where the compiler won't remove it if the input is a 
// compile time constant MIN_INT
static inline int32_t negate32(int32_t in) { return (int32_t)(~(uint32_t)in + 1); }
static inline int64_t negate64(int64_t in) { return (int64_t)(~(uint64_t)in + 1); }

static inline uint32_t safe_abs32(int32_t in)
{
    if (in < 0)
        return ~(uint32_t)in + 1;

    return (uint32_t)in;
}

static inline uint64_t safe_abs64(int64_t in)
{
    if (in < 0)
        return ~(uint64_t)in + 1;

    return (uint64_t)in;
}

// Checked casting functions
// 0 if the cast is safe, non-zero if unsafe
static inline int check_cast_int8_int32(int32_t in) { return (in < INT8_MIN || in > INT8_MAX); }
static inline int check_cast_int8_uint32(uint32_t in) { return in > INT8_MAX; }
static inline int check_cast_int8_int64(int64_t in) { return in < INT8_MIN || in > INT8_MAX; }
static inline int check_cast_int8_uint64(uint64_t in) { return (in > INT8_MAX); }
static inline int check_cast_int16_int32(int32_t in) { return in < INT16_MIN || in > INT16_MAX; }
static inline int check_cast_int16_uint32(uint32_t in) { return (in > INT16_MAX); }
static inline int check_cast_int16_int64(int64_t in) { return (in < INT16_MIN || in > INT16_MAX); }
static inline int check_cast_int16_uint64(uint64_t in) { return (in > INT16_MAX); }
static inline int check_cast_int32_uint32(uint32_t in) { return (in > INT32_MAX); }
static inline int check_cast_int32_int64(int64_t in) { return (in < INT32_MIN || in > INT32_MAX); }
static inline int check_cast_int32_uint64(uint64_t in) { return (in > INT32_MAX); }
static inline int check_cast_int64_uint64(uint64_t in) { return (in > INT64_MAX); }
static inline int check_cast_uint8_int32(int32_t in) { return (in < 0 || in > UINT8_MAX); }
static inline int check_cast_uint8_uint32(uint32_t in) { return (in > UINT8_MAX); }
static inline int check_cast_uint8_int64(int64_t in) { return (in < 0 || in > UINT8_MAX); }
static inline int check_cast_uint8_uint64(uint64_t in) { return (in > UINT8_MAX); }
static inline int check_cast_uint16_int32(int32_t in) { return (in < 0 || in > UINT16_MAX); }
static inline int check_cast_uint16_uint32(uint32_t in) { return (in > UINT16_MAX); }
static inline int check_cast_uint16_int64(int64_t in) { return (in < 0 || in > UINT16_MAX); }
static inline int check_cast_uint16_uint64(uint64_t in) { return (in > UINT16_MAX); }
static inline int check_cast_uint32_int32(int32_t in) { return (in < 0); }
static inline int check_cast_uint32_int64(int64_t in) { return (in < 0 || in > UINT32_MAX); }
static inline int check_cast_uint32_uint64(uint64_t in) { return (in > UINT32_MAX); }
static inline int check_cast_uint64_int64(int64_t in) { return (in < 0); }

static inline int8_t safe_cast_int8_int32(int32_t in)
{
    if (!check_cast_int8_int32(in))
        safe_math_fail("safe_math_fail safe_cast_int8_int32");

    return (int8_t)in;
}

static inline int8_t safe_cast_int8_uint32(uint32_t in)
{
    if (check_cast_int8_uint32(in))
        safe_math_fail("safe_math_fail safe_cast_int8_uint32");

    return (int8_t)in;
}

static inline int8_t safe_cast_int8_int64(int64_t in)
{
    if (check_cast_int8_int64(in))
        safe_math_fail("safe_math_fail safe_cast_int8_int64");

    return (int8_t)in;
}

static inline int8_t safe_cast_int8_uint64(uint64_t in)
{
    if (check_cast_int8_uint64(in))
        safe_math_fail("safe_math_fail safe_cast_int8_uint64");

    return (int8_t)in;
}

static inline int16_t safe_cast_int16_int32(int32_t in)
{
    if (check_cast_int16_int32(in))
        safe_math_fail("safe_math_fail safe_cast_int16_int32");

    return (int16_t)in;
}

static inline int16_t safe_cast_int16_uint32(uint32_t in)
{
    if (check_cast_int16_uint32(in))
        safe_math_fail("safe_math_fail safe_cast_int16_uint32");

    return (int16_t)in;
}

static inline int16_t safe_cast_int16_int64(int64_t in)
{
    if (check_cast_int16_int64(in))
        safe_math_fail("safe_math_fail safe_cast_int16_int64");

    return (int16_t)in;
}

static inline int16_t safe_cast_int16_uint64(uint64_t in)
{
    if (in > INT16_MAX)
        safe_math_fail("safe_math_fail safe_cast_int16_uint64");

    return (int16_t)in;
}

static inline int32_t safe_cast_int32_uint32(uint32_t in)
{
    if (check_cast_int32_uint32(in))
        safe_math_fail("safe_math_fail safe_cast_int32_uint32");

    return (int32_t)in;
}

static inline int32_t safe_cast_int32_int64(int64_t in)
{
    if (check_cast_int32_int64(in))
        safe_math_fail("safe_math_fail safe_cast_int32_int64");

    return (int32_t)in;
}

static inline int32_t safe_cast_int32_uint64(uint64_t in)
{
    if (check_cast_int32_uint64(in))
        safe_math_fail("safe_math_fail safe_cast_int32_uint64");

    return (int32_t)in;
}

static inline int64_t safe_cast_int64_uint64(uint64_t in)
{
    if (check_cast_int64_uint64(in))
        safe_math_fail("safe_math_fail safe_cast_int64_uint64");

    return (int64_t)in;
}

static inline uint8_t safe_cast_uint8_int32(int32_t in)
{
    if (check_cast_uint8_int32(in))
        safe_math_fail("safe_math_fail safe_cast_uint8_int32");

    return (uint8_t)in;
}

static inline uint8_t safe_cast_uint8_uint32(uint32_t in)
{
    if (check_cast_uint8_uint32(in))
        safe_math_fail("safe_math_fail safe_cast_uint8_uint32");

    return (uint8_t)in;
}

static inline uint8_t safe_cast_uint8_int64(int64_t in)
{
    if (check_cast_uint8_int64(in))
        safe_math_fail("safe_math_fail safe_cast_uint8_int64");

    return (uint8_t)in;
}

static inline uint8_t safe_cast_uint8_uint64(uint64_t in)
{
    if (check_cast_uint8_uint64(in))
        safe_math_fail("safe_math_fail safe_cast_uint8_uint64");

    return (uint8_t)in;
}

static inline uint16_t safe_cast_uint16_int32(int32_t in)
{
    if (check_cast_uint16_int32(in))
        safe_math_fail("safe_math_fail safe_cast_uint16_int32");

    return (uint16_t)in;
}

static inline uint16_t safe_cast_uint16_uint32(uint32_t in)
{
    if (check_cast_uint16_uint32(in))
        safe_math_fail("safe_math_fail safe_cast_uint16_uint32");

    return (uint16_t)in;
}

static inline uint16_t safe_cast_uint16_int64(int64_t in)
{
    if (check_cast_uint16_int64(in))
        safe_math_fail("safe_math_fail safe_cast_uint16_int64");

    return (uint16_t)in;
}

static inline uint16_t safe_cast_uint16_uint64(uint64_t in)
{
    if (check_cast_uint16_uint64(in))
        safe_math_fail("safe_math_fail safe_cast_int16_uint64");

    return (uint16_t)in;
}

static inline uint32_t safe_cast_uint32_int32(int32_t in)
{
    if (check_cast_uint32_int32(in))
        safe_math_fail("safe_math_fail safe_cast_uint32_int32");

    return (uint32_t)in;
}

static inline uint32_t safe_cast_uint32_int64(int64_t in)
{
    if (check_cast_uint32_int64(in))
        safe_math_fail("safe_math_fail safe_cast_int32_int64");

    return (uint32_t)in;
}

static inline uint32_t safe_cast_uint32_uint64(uint64_t in)
{
    if (check_cast_uint32_uint64(in))
        safe_math_fail("safe_math_fail safe_cast_uint32_uint64");

    return (uint32_t)in;
}

static inline uint64_t safe_cast_uint64_int64(int64_t in)
{
    if (check_cast_uint64_int64(in))
        safe_math_fail("safe_math_fail safe_cast_int64_uint64");

    return (uint64_t)in;
}

// Addition
/*
    For addition and multiplication, there will be checks for the following matrix:
    - int32
    - uint32
    - int64
    - uint64

    If you want to add smaller types, then do it inside the appropriate safe_cast function,
    or if adding one of the above and a smaller type, pass it into one that takes a larger
    size of the same type, for example, uint16 -> uint32.
*/

static inline int32_t safe_add_int32_int32(int32_t a, int32_t b)
{
    return safe_cast_int32_int64((int64_t)a + b);
}

static inline bool check_add_int32_int32(int32_t a, int32_t b, int32_t* ret)
{
    int64_t tmp = (int64_t)a + b;
    *ret = (int32_t)tmp;
    return check_cast_int32_int64(tmp) == 0;
}

static inline int32_t safe_add_int32_uint32(int32_t a, uint32_t b)
{
    return safe_cast_int32_int64((int64_t)a + b);
}

static inline bool check_add_int32_uint32(int32_t a, uint32_t b, int32_t* ret)
{
    int64_t tmp = (int64_t)a + b;
    *ret = (int32_t)tmp;
    return check_cast_int32_int64(tmp) == 0;
}

static inline int32_t safe_add_int32_int64(int32_t a, int64_t b)
{
    int64_t tmp = (int64_t)((uint64_t)a + (uint64_t)b);
    
    if (a >= 0)
    {
        // mixed sign cannot overflow
        if (b >= 0 && tmp < a)
            safe_math_fail("safe_math_fail safe_add_int32_int64");
    }
    else
    {
        // lhs negative
        if (b < 0 && tmp > a)
            safe_math_fail("safe_math_fail safe_add_int32_int64");
    }

    return safe_cast_int32_int64(tmp);
}

static inline bool check_add_int32_int64(int32_t a, int64_t b, int32_t* ret)
{
    int64_t tmp = (int64_t)((uint64_t)a + (uint64_t)b);
    *ret = (int32_t)tmp;

    if (a >= 0)
    {
        // mixed sign cannot overflow
        if (b >= 0 && tmp < a)
            return false;
    }
    else
    {
        // lhs negative
        if (b < 0 && tmp > a)
            return false;
    }

    return check_cast_int32_int64(tmp) == 0;
}

static inline int32_t safe_add_int32_uint64(int32_t a, uint64_t b)
{
    if ((uint32_t)(b >> 32) == 0)
    {
        // Now it just happens to work out that the standard behavior does what we want
        // Adding explicit casts to show exactly what's happening here
        uint32_t tmp = (uint32_t)a + (uint32_t)b;

        if ((int32_t)tmp >= a)
        {
            return (int32_t)tmp;
        }
    }

    safe_math_fail("safe_math_fail safe_add_int32_uint64");
}

static inline bool check_add_int32_uint64(int32_t a, uint64_t b, int32_t* ret)
{
    if ((uint32_t)(b >> 32) == 0)
    {
        // Now it just happens to work out that the standard behavior does what we want
        // Adding explicit casts to show exactly what's happening here
        uint32_t tmp = (uint32_t)a + (uint32_t)b;
        *ret = (int32_t)tmp;

        if ((int32_t)tmp >= a)
        {
            return true;
        }
    }

    return false;
}

static inline uint32_t safe_add_uint32_int32(uint32_t a, int32_t b)
{
    return safe_cast_uint32_int64((int64_t)a + b);
}

static inline bool check_add_uint32_int32(uint32_t a, int32_t b, uint32_t* ret)
{
    int64_t tmp = (int64_t)a + b;
    *ret = (uint32_t)tmp;
    return check_cast_uint32_int64(tmp) == 0;
}

static inline uint32_t safe_add_uint32_uint32(uint32_t a, uint32_t b)
{
    uint32_t tmp = a + b;
    
    if (tmp < a)
    {
        safe_math_fail("safe_math_fail safe_add_uint32_uint32");
    }

    return tmp;
}

static inline bool check_add_uint32_uint32(uint32_t a, uint32_t b, uint32_t* ret)
{
    uint32_t tmp = a + b;
    *ret = tmp;
    return tmp >= a;
}

static inline uint32_t safe_add_uint32_int64(uint32_t a, int64_t b)
{
    if (b < 0)
    {
        if (a >= safe_abs64(b)) //negation is safe, since rhs is 64-bit
        {
            return (uint32_t)(a + b);
        }
    }
    else
    {
        // now we know that rhs can be safely cast into an std::uint64_t
        uint64_t tmp = (uint64_t)a + (uint64_t)b;

        // special case - rhs cannot be larger than 0x7fffffffffffffff, lhs cannot be larger than 0xffffffff
        // it is not possible for the operation above to overflow, so just check max
        return safe_cast_uint32_uint64(tmp);
    }

    safe_math_fail("safe_math_fail safe_add_uint32_int64");
}

static inline bool check_add_uint32_int64(uint32_t a, int64_t b, uint32_t* ret)
{
    if (b < 0)
    {
        if (a >= safe_abs64(b)) //negation is safe, since rhs is 64-bit
        {
            *ret = (uint32_t)(a + b);
            return true;
        }
    }
    else
    {
        // now we know that rhs can be safely cast into an std::uint64_t
        uint64_t tmp = (uint64_t)a + (uint64_t)b;

        // special case - rhs cannot be larger than 0x7fffffffffffffff, lhs cannot be larger than 0xffffffff
        // it is not possible for the operation above to overflow, so just check max
        *ret = (uint32_t)tmp;
        return check_cast_uint32_uint64(tmp) == 0;
    }

    return false;
}

static inline uint32_t safe_add_uint32_uint64(uint32_t a, uint64_t b)
{
    uint64_t tmp = (uint64_t)a + b;
    
    if (tmp >= a && tmp <= UINT32_MAX)
    {
        return (uint32_t)tmp;
    }

    safe_math_fail("safe_math_fail safe_add_uint32_uint64");
}

static inline bool check_add_uint32_uint64(uint32_t a, uint64_t b, uint32_t* ret)
{
    uint64_t tmp = (uint64_t)a + b;
    *ret = (uint32_t)tmp;

    return (tmp >= a && tmp <= UINT32_MAX);
}

static inline int64_t safe_add_int64_int32(int64_t a, int32_t b)
{
    int64_t tmp = (int64_t)((uint64_t)a + (uint64_t)b);

    if (a >= 0)
    {
        // mixed sign cannot overflow
        if (b >= 0 && tmp < a)
            safe_math_fail("safe_math_fail safe_add_int64_int32");
    }
    else
    {
        // lhs negative
        if (b < 0 && tmp > a)
            safe_math_fail("safe_math_fail safe_add_int64_int32");
    }

    return tmp;
}

static inline bool check_add_int64_int32(int64_t a, int32_t b, int64_t* ret)
{
    int64_t tmp = (int64_t)((uint64_t)a + (uint64_t)b);
    *ret = tmp;

    if (a >= 0)
    {
        // mixed sign cannot overflow
        if (b >= 0 && tmp < a)
            return false;
    }
    else
    {
        // lhs negative
        if (b < 0 && tmp > a)
            return false;
    }

    return true;
}

static inline int64_t safe_add_int64_uint32(int64_t a, uint32_t b)
{
    uint64_t tmp = (uint64_t)a + (uint64_t)b;

    if ((int64_t)tmp >= a)
    {
        return (int64_t)tmp;
    }

    safe_math_fail("safe_math_fail safe_add_int64_uint32");
}

static inline bool check_add_int64_uint32(int64_t a, uint32_t b, int64_t* ret)
{
    uint64_t tmp = (uint64_t)a + (uint64_t)b;
    *ret = (int64_t)tmp;

    return ((int64_t)tmp >= a);
}

static inline int64_t safe_add_int64_int64(int64_t a, int64_t b)
{
    int64_t tmp = (int64_t)((uint64_t)a + (uint64_t)b);

    if (a >= 0)
    {
        // mixed sign cannot overflow
        if (b >= 0 && tmp < a)
            safe_math_fail("safe_math_fail safe_add_int64_int64");
    }
    else
    {
        // lhs negative
        if (b < 0 && tmp > a)
            safe_math_fail("safe_math_fail safe_add_int64_int64");
    }

    return tmp;
}

static inline bool check_add_int64_int64(int64_t a, int64_t b, int64_t* ret)
{
    int64_t tmp = (int64_t)((uint64_t)a + (uint64_t)b);
    *ret = tmp;

    if (a >= 0)
    {
        // mixed sign cannot overflow
        if (b >= 0 && tmp < a)
            return false;
    }
    else
    {
        // lhs negative
        if (b < 0 && tmp > a)
            return false;
    }

    return true;
}

static inline int64_t safe_add_int64_uint64(int64_t a, uint64_t b)
{
    uint64_t tmp = (uint64_t)a + b;

    if ((int64_t)tmp >= a)
    {
        return (int64_t)tmp;
    }

    safe_math_fail("safe_math_fail safe_add_int64_uint64");
}

static inline bool check_add_int64_uint64(int64_t a, uint64_t b, int64_t* ret)
{
    uint64_t tmp = (uint64_t)a + b;
    *ret = (int64_t)tmp;

    return ((int64_t)tmp >= a);
}

static inline uint64_t safe_add_uint64_int32(uint64_t a, int32_t b)
{
    uint64_t tmp = 0;

    if (b < 0)
    {
        // So we're effectively subtracting
        tmp = safe_abs32(b);

        if (tmp <= a)
        {
            return a - tmp;
        }
    }
    else
    {
        // now we know that rhs can be safely cast into an std::uint64_t
        tmp = (uint64_t)a + (uint64_t)b;

        // We added and it did not become smaller
        if (tmp >= a)
        {
            return tmp;
        }
    }

    safe_math_fail("safe_math_fail safe_add_uint64_int32");
}

static inline bool check_add_uint64_int32(uint64_t a, int32_t b, uint64_t* ret)
{
    uint64_t tmp = 0;

    if (b < 0)
    {
        // So we're effectively subtracting
        tmp = safe_abs32(b);

        if (tmp <= a)
        {
            *ret = a - tmp;
            return true;
        }
    }
    else
    {
        // now we know that rhs can be safely cast into an std::uint64_t
        tmp = (uint64_t)a + (uint64_t)b;

        // We added and it did not become smaller
        if (tmp >= a)
        {
            *ret = tmp;
            return true;
        }
    }

    return false;
}


static inline uint64_t safe_add_uint64_uint32(uint64_t a, uint32_t b)
{
    uint64_t tmp = (uint64_t)a + (uint64_t)b;

    // We added and it didn't get smaller
    if (tmp >= a)
    {
        return tmp;
    }

    safe_math_fail("safe_math_fail safe_add_uint64_uint32");
}

static inline bool check_add_uint64_uint32(uint64_t a, uint32_t b, uint64_t* ret)
{
    uint64_t tmp = (uint64_t)a + (uint64_t)b;
    *ret = tmp;

    // We added and it didn't get smaller
    return (tmp >= a);
}

static inline uint64_t safe_add_uint64_int64(uint64_t a, int64_t b)
{
    uint64_t tmp = 0;

    if (b < 0)
    {
        // So we're effectively subtracting
        tmp = safe_abs64(b);

        if (tmp <= a)
        {
            return a - tmp;
        }
    }
    else
    {
        // now we know that rhs can be safely cast into an std::uint64_t
        tmp = (uint64_t)a + (uint64_t)b;

        // We added and it did not become smaller
        if (tmp >= a)
        {
            return tmp;
        }
    }

    safe_math_fail("safe_math_fail safe_add_uint64_int64");
}

static inline bool check_add_uint64_int64(uint64_t a, int64_t b, uint64_t* ret)
{
    uint64_t tmp = 0;

    if (b < 0)
    {
        // So we're effectively subtracting
        tmp = safe_abs64(b);

        if (tmp <= a)
        {
            *ret = a - tmp;
            return true;
        }
    }
    else
    {
        // now we know that rhs can be safely cast into an std::uint64_t
        tmp = (uint64_t)a + (uint64_t)b;

        // We added and it did not become smaller
        if (tmp >= a)
        {
            *ret = tmp;
            return true;
        }
    }

    return false;
}

static inline uint64_t safe_add_uint64_uint64(uint64_t a, uint64_t b)
{
    uint64_t tmp = a + b;

    if(tmp < a)
        safe_math_fail("safe_math_fail safe_add_uint64_uint64");

    return tmp;
}

static inline bool check_add_uint64_uint64(uint64_t a, uint64_t b, uint64_t* ret)
{
    uint64_t tmp = a + b;
    *ret = tmp;
    return (tmp >= a);
}

// As we're working in C, use defines
// It would be nice to use an enum, but the compiler 
// will complain that it isn't a proper C++ enum
#define SAFE_INT_MUL_FAIL 0
#define SAFE_INT_MUL_SUCCESS 1

// Multiplication primatives
#if SAFEINT_MULTIPLY_METHOD == SAFEINT_MULTIPLY_INT128

static inline int MultiplyUint64(uint64_t a, uint64_t b, uint64_t* pRet)
{
    unsigned __int128 tmp = (unsigned __int128)a * (unsigned __int128)b;

    if ((tmp >> 64) == 0)
    {
        *pRet = (uint64_t)tmp;
        return SAFE_INT_MUL_SUCCESS;
    }

    return SAFE_INT_MUL_FAIL;
}

static inline int MultiplyInt64(int64_t a, int64_t b, int64_t* pRet)
{
    __int128 tmp = (__int128)a * (__int128)b;
    int64_t tmp_high = (int64_t)((unsigned __int128)tmp >> 64);
    *pRet = (int64_t)tmp;

    // If only one input is negative, result must be negative, or zero
    if ((a ^ b) < 0)
    {
        if ((tmp_high == -1 && *pRet < 0) ||
            (tmp_high == 0 && *pRet == 0))
        {
            return SAFE_INT_MUL_SUCCESS;
        }
    }
    else
    {
        if (tmp_high == 0 && (uint64_t)*pRet <= (uint64_t)INT64_MAX)
        {
            return SAFE_INT_MUL_SUCCESS;
        }
    }

    return SAFE_INT_MUL_FAIL;
}

#elif SAFEINT_MULTIPLY_METHOD == SAFEINT_MULTIPLY_INTRINSICS // Implies Visual Studio compiler

// As usual, unsigned is easy
static inline int MultiplyUint64(uint64_t a, uint64_t b, uint64_t * pRet)
{
    uint64_t ulHigh = 0;
    *pRet = _umul128(a, b, &ulHigh);
    return ulHigh == 0 ? SAFE_INT_MUL_SUCCESS : SAFE_INT_MUL_FAIL;
}

// Signed, is not so easy
static inline int MultiplyInt64(int64_t a, int64_t b, int64_t* pRet)
{
    int64_t llHigh = 0;
    *pRet = _mul128(a, b, &llHigh);

    // Now we need to figure out what we expect
    // If llHigh is 0, then treat *pRet as unsigned
    // If llHigh is < 0, then treat *pRet as signed

    if ((a ^ b) < 0)
    {
        // Negative (or zero) result expected
        if (llHigh == -1 && *pRet < 0 ||
            llHigh == 0 && *pRet == 0)
        {
            // Everything is within range
            return SAFE_INT_MUL_SUCCESS;
        }
    }
    else
    {
        // Result should be positive
        // Check for overflow
        if (llHigh == 0 && (uint64_t)*pRet <= (uint64_t)INT64_MAX)
            return SAFE_INT_MUL_SUCCESS;
    }
    return SAFE_INT_MUL_FAIL;
}
#elif SAFEINT_MULTIPLY_METHOD == SAFEINT_MULTIPLY_BUILTIN // Implies gcc or clang

static inline int MultiplyUint64(uint64_t a, uint64_t b, uint64_t* pRet)
{
    return !__builtin_umulll_overflow(a, b, (unsigned long long*)pRet) ? SAFE_INT_MUL_SUCCESS : SAFE_INT_MUL_FAIL;
}

static inline int MultiplyInt64(int64_t a, int64_t b, int64_t* pRet)
{
    return !__builtin_smulll_overflow(a, b, (long long*)pRet) ? SAFE_INT_MUL_SUCCESS : SAFE_INT_MUL_FAIL;
}

#elif SAFEINT_MULTIPLY_METHOD == SAFEINT_MULTIPLY_MATH // Just going to have to do the math...

static inline int MultiplyUint64(uint64_t a, uint64_t b, uint64_t* pRet)
{
    uint32_t a_high = a >> 32;
    uint32_t a_low = (uint32_t)a;
    uint32_t b_high = b >> 32;
    uint32_t b_low = (uint32_t)b;
    uint64_t tmp = 0;
    uint64_t tmp2 = 0;

    /*
    * Now we have the equivalent of (a_high * 2^32 + a_low) * (b_high * 2^32 + b_low)
    * Expanding:
    * result = a_high * b_high * 2^64 + a_high * b_low * 2^32 + b_high * a_low * 2^32 + a_low * b_low
    * We now get to short circult some things - if a_high > 0 && b_high > 0, fail
    * and this then implies that only one of the two middle expressions must be evaluated and checked if the result is >= 2^32
    * finally, do the last term, check addition
    */

    if (a_high > 0 && b_high > 0)
    {
        return SAFE_INT_MUL_FAIL;
    }

    if (a_high > 0)
    {
        tmp = (uint64_t)a_high * b_low;
    }
    else
    {
        tmp = (uint64_t)b_high * a_low;
    }

    if (tmp >> 32 != 0)
    {
        return SAFE_INT_MUL_FAIL;
    }

    tmp2 = (uint64_t)a_low * b_low;
    *pRet = (tmp << 32) + tmp2;
    return *pRet >= tmp2 ? SAFE_INT_MUL_SUCCESS : SAFE_INT_MUL_FAIL;
}

static inline int MultiplyInt64(int64_t a, int64_t b, int64_t* pRet)
{
    bool aNegative = false;
    bool bNegative = false;

    uint64_t tmp = 0;
    int64_t a1 = a;
    int64_t b1 = b;

    if (a1 < 0)
    {
        aNegative = true;
        a1 = (int64_t)safe_abs64(a1);
    }

    if (b1 < 0)
    {
        bNegative = true;
        b1 = (int64_t)safe_abs64(b);
    }

    if (MultiplyUint64((uint64_t)a1, (uint64_t)b1, &tmp))
    {
        // The unsigned multiplication didn't overflow
        if (aNegative ^ bNegative)
        {
            // Result must be negative
            if (tmp <= (uint64_t)INT64_MIN)
            {
                *pRet = (int64_t)negate64((int64_t)tmp);
                return SAFE_INT_MUL_SUCCESS;
            }
        }
        else
        {
            // Result must be positive
            if (tmp <= (uint64_t)INT64_MAX)
            {
                *pRet = (int64_t)tmp;
                return SAFE_INT_MUL_SUCCESS;
            }
        }
    }

    return SAFE_INT_MUL_FAIL;
}

#else // Shouldn't happen, go find out what's broken
// If you are aware of intrinsics for some other platform, please file an issue
# error Intrinsics enabled, no available intrinics defined
#endif

static inline int32_t safe_mul_int32_int32(int32_t a, int32_t b)
{
    int64_t tmp = (int64_t)a * (int64_t)b;
    return safe_cast_int32_int64(tmp);
}

static inline bool check_mul_int32_int32(int32_t a, int32_t b, int32_t* ret)
{
    int64_t tmp = (int64_t)a * (int64_t)b;
    *ret = (int32_t)tmp;
    return check_cast_int32_int64(tmp) == 0;
}

static inline int32_t safe_mul_int32_uint32(int32_t a, uint32_t b)
{
    int64_t tmp = (int64_t)a * (int64_t)b;
    return safe_cast_int32_int64(tmp);
}

static inline bool check_mul_int32_uint32(int32_t a, uint32_t b, int32_t* ret)
{
    int64_t tmp = (int64_t)a * (int64_t)b;
    *ret = (int32_t)tmp;
    return check_cast_int32_int64(tmp) == 0;
}

static inline int32_t safe_mul_int32_int64(int32_t a, int64_t b)
{
    int64_t tmp = 0;

    if (MultiplyInt64((int64_t)a, b, &tmp))
    {
        return safe_cast_int32_int64(tmp);
    }

    safe_math_fail("safe_math_fail safe_mul_int32_int64");
}

static inline bool check_mul_int32_int64(int32_t a, int64_t b, int32_t* ret)
{
    int64_t tmp = 0;

    if (MultiplyInt64((int64_t)a, b, &tmp))
    {
        *ret = (int32_t)tmp;
        return check_cast_int32_int64(tmp) == 0;
    }

    return false;
}

static inline int32_t safe_mul_int32_uint64(int32_t a, uint64_t b)
{
    uint64_t tmp = 0;
    if (a < 0)
    {
        // Flip sign, use the unsigned function
        uint64_t a2 = safe_abs64(a);
        if (MultiplyUint64(a2, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= (uint64_t)INT32_MAX + 1)
        {
            // Not too big, flip it back
            return (int32_t)(tmp + 1);
        }
    }
    else
    {
        if (MultiplyUint64((uint64_t)a, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= INT32_MAX)
        {
            return (int32_t)tmp;
        }
    }
 
    safe_math_fail("safe_math_fail safe_mul_int32_uint64");
}

static inline bool check_mul_int32_uint64(int32_t a, uint64_t b, int32_t* ret)
{
    uint64_t tmp = 0;
    if (a < 0)
    {
        // Flip sign, use the unsigned function
        uint64_t a2 = safe_abs64(a);
        if (MultiplyUint64(a2, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= (uint64_t)INT32_MAX + 1)
        {
            // Not too big, flip it back
            *ret = (int32_t)(tmp + 1);
            return true;
        }
    }
    else
    {
        if (MultiplyUint64((uint64_t)a, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= INT32_MAX)
        {
            *ret = (int32_t)tmp;
            return true;
        }
    }

    return false;
}

static inline uint32_t safe_mul_uint32_int32(uint32_t a, int32_t b)
{
    int64_t tmp = (int64_t)a * (int64_t)b;
    return safe_cast_uint32_int64(tmp);
}

static inline bool check_mul_uint32_int32(uint32_t a, int32_t b, uint32_t* ret)
{
    int64_t tmp = (int64_t)a * (int64_t)b;
    *ret = (uint32_t)tmp;
    return check_cast_uint32_int64(tmp) == 0;
}

static inline uint32_t safe_mul_uint32_uint32(uint32_t a, uint32_t b)
{
    uint64_t tmp = (uint64_t)a * (uint64_t)b;
    return safe_cast_uint32_uint64(tmp);
}

static inline bool check_mul_uint32_uint32(uint32_t a, uint32_t b, uint32_t* ret)
{
    uint64_t tmp = (uint64_t)a * (uint64_t)b;
    *ret = (uint32_t)tmp;
    return check_cast_uint32_uint64(tmp) == 0;
}

static inline uint32_t safe_mul_uint32_int64(uint32_t a, int64_t b)
{
    int64_t tmp = 0;

    if (MultiplyInt64((int64_t)a, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= UINT32_MAX && tmp >= 0)
    {
        return (uint32_t)tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_uint32_int64");
}

static inline bool check_mul_uint32_int64(uint32_t a, int64_t b, uint32_t* ret)
{
    int64_t tmp = 0;

    if (MultiplyInt64((int64_t)a, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= UINT32_MAX && tmp >= 0)
    {
        *ret = (uint32_t)tmp;
        return true;
    }

    return false;
}

static inline uint32_t safe_mul_uint32_uint64(uint32_t a, uint64_t b)
{
    uint64_t tmp = 0;

    if (MultiplyUint64((uint64_t)a, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= UINT32_MAX)
    {
        return (uint32_t)tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_uint32_uint64");
}

static inline bool check_mul_uint32_uint64(uint32_t a, uint64_t b, uint32_t* ret)
{
    uint64_t tmp = 0;

    if (MultiplyUint64((uint64_t)a, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= UINT32_MAX)
    {
        *ret = (uint32_t)tmp;
        return true;
    }

    return false;
}

static inline int64_t safe_mul_int64_int32(int64_t a, int32_t b)
{
    int64_t tmp = 0;

    if (MultiplyInt64(a, (int64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        return tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_int64_int32");
}

static inline bool check_mul_int64_int32(int64_t a, int32_t b, int64_t* ret)
{
    int64_t tmp = 0;

    if (MultiplyInt64(a, (int64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        *ret = tmp;
        return true;
    }

    return false;
}

static inline int64_t safe_mul_int64_uint32(int64_t a, uint32_t b)
{
    int64_t tmp = 0;

    if (MultiplyInt64(a, (int64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        return tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_int64_uint32");
}

static inline bool check_mul_int64_uint32(int64_t a, uint32_t b, int64_t* ret)
{
    int64_t tmp = 0;

    if (MultiplyInt64(a, (int64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        *ret = tmp;
        return true;
    }

    return false;
}

static inline int64_t safe_mul_int64_int64(int64_t a, int64_t b)
{
    int64_t tmp = 0;

    if (MultiplyInt64(a, b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        return tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_int64_int64");
}

static inline bool check_mul_int64_int64(int64_t a, int64_t b, int64_t* ret)
{
    int64_t tmp = 0;

    if (MultiplyInt64(a, b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        *ret = tmp;
        return true;
    }

    return false;
}

static inline int64_t safe_mul_int64_uint64(int64_t a, uint64_t b)
{
    uint64_t tmp = 0;

    if (a < 0)
    {
        uint64_t a2 = safe_abs64(a);

        if (MultiplyUint64(a2, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= (uint64_t)0x8000000000000000)
        {
            return negate64((int64_t)tmp);
        }
    }
    else
    {
        if (MultiplyUint64((uint64_t)a, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= (uint64_t)INT64_MAX)
        {
            return (int64_t)tmp;
        }
    }

    safe_math_fail("safe_math_fail safe_mul_int64_uint64");
}

static inline bool check_mul_int64_uint64(int64_t a, uint64_t b, int64_t* ret)
{
    uint64_t tmp = 0;

    if (a < 0)
    {
        uint64_t a2 = safe_abs64(a);

        if (MultiplyUint64(a2, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= (uint64_t)0x8000000000000000)
        {
            *ret = negate64((int64_t)tmp);
            return true;
        }
    }
    else
    {
        if (MultiplyUint64((uint64_t)a, b, &tmp) == SAFE_INT_MUL_SUCCESS && tmp <= (uint64_t)INT64_MAX)
        {
            *ret = (int64_t)tmp;
            return true;
        }
    }

    return false;
}

static inline uint64_t safe_mul_uint64_int32(uint64_t a, int32_t b)
{
    uint64_t tmp;

    if (b < 0)
    {
        if (a == 0)
            return 0;

        safe_math_fail("safe_math_fail safe_mul_uint64_int32");
    }
   
    if (MultiplyUint64(a, (uint64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        return tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_uint64_int32");
}

static inline bool check_mul_uint64_int32(uint64_t a, int32_t b, uint64_t* ret)
{
    uint64_t tmp;

    if (b < 0)
    {
        if (a == 0)
        {
            *ret = 0;
            return true;
        }

        return false;
    }

    if (MultiplyUint64(a, (uint64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        *ret = tmp;
        return true;
    }

    return false;
}

static inline uint64_t safe_mul_uint64_uint32(uint64_t a, uint32_t b)
{
    uint64_t tmp;

    if (MultiplyUint64(a, (uint64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        return tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_uint64_uint32");
}

static inline bool check_mul_uint64_uint32(uint64_t a, uint32_t b, uint64_t* ret)
{
    uint64_t tmp;

    if (MultiplyUint64(a, (uint64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        *ret = tmp;
        return true;
    }

    return false;
}

static inline uint64_t safe_mul_uint64_int64(uint64_t a, int64_t b)
{
    uint64_t tmp;

    if (b < 0)
    {
        if (a == 0)
            return 0;

        safe_math_fail("safe_math_fail safe_mul_uint64_int32");
    }

    if (MultiplyUint64(a, (uint64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        return tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_uint64_int64");
}

static inline bool check_mul_uint64_int64(uint64_t a, int64_t b, uint64_t* ret)
{
    uint64_t tmp;

    if (b < 0)
    {
        if (a == 0)
        {
            *ret = 0;
            return true;
        }

        return false;
    }

    if (MultiplyUint64(a, (uint64_t)b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        *ret = tmp;
        return true;
    }

    return false;
}

static inline uint64_t safe_mul_uint64_uint64(uint64_t a, uint64_t b)
{
    uint64_t tmp;

    if (MultiplyUint64(a, b, &tmp) == SAFE_INT_MUL_SUCCESS)
    {
        return tmp;
    }

    safe_math_fail("safe_math_fail safe_mul_uint64_uint64");
}

static inline bool check_mul_uint64_uint64(uint64_t a, uint64_t b, uint64_t* ret)
{
    return (MultiplyUint64(a, b, ret) == SAFE_INT_MUL_SUCCESS);
}

static inline int32_t safe_div_int32_int32(int32_t a, int32_t b)
{
    if (b != 0 && !(a == INT32_MIN && b == -1))
    {
        return a / b;
    }
    safe_math_fail("safe_math_fail safe_div_int32_int32");
}

static inline bool check_div_int32_int32(int32_t a, int32_t b, int32_t* ret)
{
    if (b != 0 && !(a == INT32_MIN && b == -1))
    {
        *ret = a / b;
        return true;
    }
    return false;
}

static inline int32_t safe_div_int32_uint32(int32_t a, uint32_t b)
{
    if (b != 0)
    {
        return (int32_t)((int64_t)a / (int64_t)b);
    }
    safe_math_fail("safe_math_fail safe_div_int32_uint32");
}

static inline bool check_div_int32_uint32(int32_t a, uint32_t b, int32_t* ret)
{
    if (b != 0)
    {
        *ret = (int32_t)((int64_t)a / (int64_t)b);
        return true;
    }

    return false;
}

static inline int32_t safe_div_int32_int64(int32_t a, int64_t b)
{
    if (b != 0 && !(a == INT32_MIN && b == -1))
    {
        return (int32_t)(a / b);
    }
    safe_math_fail("safe_math_fail safe_div_int32_int64");
}

static inline bool check_div_int32_int64(int32_t a, int64_t b, int32_t* ret)
{
    if (b != 0 && !(a == INT32_MIN && b == -1))
    {
        *ret = (int32_t)(a / b);
        return true;
    }

    return false;
}

static inline int32_t safe_div_int32_uint64(int32_t a, uint64_t b)
{
    if (b == 0)
    {
        safe_math_fail("safe_math_fail safe_div_int32_uint64");
    }

    if (a > 0)
    {
        return (int32_t)((uint64_t)a / b);
    }
    else
    {
        uint64_t a2 = (uint64_t)safe_abs32(a);
        a2 /= b;
        return (int32_t)negate32((int32_t)a2);
    }
}

static inline bool check_div_int32_uint64(int32_t a, uint64_t b, int32_t* ret)
{
    if (b == 0)
    {
        return false;
    }

    if (a > 0)
    {
        *ret = (int32_t)((uint64_t)a / b);
        return true;
    }
    else
    {
        uint64_t a2 = (uint64_t)safe_abs32(a);
        a2 /= b;
        *ret = (int32_t)negate32((int32_t)a2);
        return true;
    }
}

static inline uint32_t safe_div_uint32_int32(uint32_t a, int32_t b)
{
    // Follow original SafeInt logic for this case
    if (b == 0) // div 0 always a problem
    {
        safe_math_fail("safe_math_fail safe_div_uint32_int32");
    }

    if (a == 0) // zero divided by anything is zero
    {
        return 0;
    }

    if (b > 0) // if b is positive, just do the math
    {
        return (a / (uint32_t)b);
    }
    else // now have to check magnitude
    {
        uint32_t tmp = safe_abs32(b);

        if (a < tmp)
        {
            return 0;
        }
    }

    safe_math_fail("safe_math_fail safe_div_uint32_int32");
}

static inline bool check_div_uint32_int32(uint32_t a, int32_t b, uint32_t* ret)
{
    // Follow original SafeInt logic for this case
    if (b == 0) // div 0 always a problem
    {
        return false;
    }

    if (a == 0) // zero divided by anything is zero
    {
        *ret = 0;
        return true;
    }

    if (b > 0) // if b is positive, just do the math
    {
        *ret = (a / (uint32_t)b);
        return true;
    }
    else // now have to check magnitude
    {
        uint32_t tmp = safe_abs32(b);

        if (a < tmp)
        {
            *ret = 0;
            return true;
        }
    }

    return false;
}

static inline uint32_t safe_div_uint32_uint32(uint32_t a, uint32_t b)
{
    if (b > 0)
    {
        return (uint32_t)(a / b);
    }
    safe_math_fail("safe_math_fail safe_div_uint32_uint32");
}

static inline bool check_div_uint32_uint32(uint32_t a, uint32_t b, uint32_t* ret)
{
    if (b > 0)
    {
        *ret = (uint32_t)(a / b);
        return true;
    }

    return false;
}

static inline uint32_t safe_div_uint32_int64(uint32_t a, int64_t b)
{
    // Follow original SafeInt logic for this case
    if (b == 0) // div 0 always a problem
    {
        safe_math_fail("safe_math_fail safe_div_uint32_int64");
    }

    if (a == 0) // zero divided by anything is zero
    {
        return 0;
    }

    if (b > 0) // if b is positive, just do the math
    {
        return (uint32_t)(a / b);
    }
    else // now have to check magnitude
    {
        uint64_t tmp = safe_abs64(b);

        if (a < tmp)
        {
            return 0;
        }
    }

    safe_math_fail("safe_math_fail safe_div_uint32_int64");
}

static inline bool check_div_uint32_int64(uint32_t a, int64_t b, uint32_t* ret)
{
    // Follow original SafeInt logic for this case
    if (b == 0) // div 0 always a problem
    {
        return false;
    }

    if (a == 0) // zero divided by anything is zero
    {
        *ret = 0;
        return true;
    }

    if (b > 0) // if b is positive, just do the math
    {
        *ret = (uint32_t)(a / b);
        return true;
    }
    else // now have to check magnitude
    {
        uint64_t tmp = safe_abs64(b);

        if (a < tmp)
        {
            *ret = 0;
            return true;
        }
    }

    return false;
}

static inline uint32_t safe_div_uint32_uint64(uint32_t a, uint64_t b)
{
    if (b > 0)
    {
        return (uint32_t)(a / b);
    }
    safe_math_fail("safe_math_fail safe_div_uint32_uint64");
}

static inline bool check_div_uint32_uint64(uint32_t a, uint64_t b, uint32_t* ret)
{
    if (b > 0)
    {
        *ret = (uint32_t)(a / b);
        return true;
    }
    return false;
}

static inline int64_t safe_div_int64_int32(int64_t a, int32_t b)
{
    if(b == 0 || (b == -1 && a == INT64_MIN))
        safe_math_fail("safe_math_fail safe_div_int64_int32");

    return a / b;
}

static inline bool check_div_int64_int32(int64_t a, int32_t b, int64_t* ret)
{
    if (b == 0 || (b == -1 && a == INT64_MIN))
        return false;

    *ret = a / b;
    return true;
}

static inline int64_t safe_div_int64_uint32(int64_t a, uint32_t b)
{
    if (b == 0)
        safe_math_fail("safe_math_fail safe_div_int64_int32");

    return a / b;
}

static inline bool check_div_int64_uint32(int64_t a, uint32_t b, int64_t* ret)
{
    if (b == 0)
        return false;

    *ret = a / b;
    return true;
}

static inline int64_t safe_div_int64_int64(int64_t a, int64_t b)
{
    if (b == 0 || (b == -1 && a == INT64_MIN))
        safe_math_fail("safe_math_fail safe_div_int64_int32");

    return a / b;
}

static inline bool check_div_int64_int64(int64_t a, int64_t b, int64_t* ret)
{
    if (b == 0 || (b == -1 && a == INT64_MIN))
        return false;

    *ret = a / b;
    return true;

}

static inline int64_t safe_div_int64_uint64(int64_t a, uint64_t b)
{
    if (b == 0)
        safe_math_fail("safe_math_fail safe_div_int64_int32");

    if(a >= 0)
    {
        return (int64_t)((uint64_t)a / b);
    }
    else
    {
        // Need to get the magnitude, divide, and then negate
        uint64_t tmp = safe_abs64(a);
        tmp /= b;
        return negate64((int64_t)tmp);
    }
}

static inline bool check_div_int64_uint64(int64_t a, uint64_t b, int64_t* ret)
{
    if (b == 0)
        return false;

    if(a >= 0)
    {
        *ret = (int64_t)((uint64_t)a / b);
    }
    else
    {
        // Need to get the magnitude, divide, and then negate
        uint64_t tmp = safe_abs64(a);
        tmp /= b;
        *ret = negate64((int64_t)tmp);
    }
        return true;
}

static inline uint64_t safe_div_uint64_int32(uint64_t a, int32_t b)
{
    // Follow original SafeInt logic for this case
    if (b == 0) // div 0 always a problem
    {
        safe_math_fail("safe_math_fail safe_div_int64_int32");
    }

    if (a == 0) // zero divided by anything is zero
    {
        return 0;
    }

    if (b > 0) // if b is positive, just do the math
    {
        return a / (uint64_t)b;
    }
    else // now have to check magnitude
    {
        uint32_t tmp = safe_abs32(b);

        if (a < tmp)
        {
            return 0;
        }
    }

    safe_math_fail("safe_math_fail safe_div_int64_int32");
}

static inline bool check_div_uint64_int32(uint64_t a, int32_t b, uint64_t* ret)
{
    // Follow original SafeInt logic for this case
    if (b == 0) // div 0 always a problem
    {
        return false;
    }

    if (a == 0) // zero divided by anything is zero
    {
        *ret = 0;
        return true;
    }

    if (b > 0) // if b is positive, just do the math
    {
        *ret = a / (uint64_t)b;
        return true;
    }
    else // now have to check magnitude
    {
        uint32_t tmp = safe_abs32(b);

        if (a < tmp)
        {
            *ret = 0;
            return true;
        }
    }

    return false;
}

static inline uint64_t safe_div_uint64_uint32(uint64_t a, uint32_t b)
{
    if (b != 0)
        return a / b;

    safe_math_fail("safe_math_fail safe_div_int64_uint32");
}

static inline bool check_div_uint64_uint32(uint64_t a, uint32_t b, uint64_t* ret)
{
    if (b != 0)
    {
        *ret = a / b;
        return true;
    }
    
    return false;
}

static inline uint64_t safe_div_uint64_int64(uint64_t a, int64_t b)
{
    // Follow original SafeInt logic for this case
    if (b == 0) // div 0 always a problem
    {
        safe_math_fail("safe_math_fail safe_div_int64_int32");
    }

    if (a == 0) // zero divided by anything is zero
    {
        return 0;
    }

    if (b > 0) // if b is positive, just do the math
    {
        return a / (uint64_t)b;
    }
    else // now have to check magnitude
    {
        uint64_t tmp = safe_abs64(b);

        if (a < tmp)
        {
            return 0;
        }
    }

    safe_math_fail("safe_math_fail safe_div_int64_int32");
}

static inline bool check_div_uint64_int64(uint64_t a, int64_t b, uint64_t* ret)
{
    // Follow original SafeInt logic for this case
    if (b == 0) // div 0 always a problem
    {
        return false;
    }

    if (a == 0) // zero divided by anything is zero
    {
        *ret = 0;
        return true;
    }

    if (b > 0) // if b is positive, just do the math
    {
        *ret = a / (uint64_t)b;
        return true;
    }
    else // now have to check magnitude
    {
        uint64_t tmp = safe_abs64(b);

        if (a < tmp)
        {
            *ret = 0;
            return true;
        }
    }

    return false;
}

static inline uint64_t safe_div_uint64_uint64(uint64_t a, uint64_t b)
{
    if (b != 0)
        return a / b;

    safe_math_fail("safe_math_fail safe_div_int64_uint32");
}

static inline bool check_div_uint64_uint64(uint64_t a, uint64_t b, uint64_t* ret)
{
    if (b != 0)
    {
        *ret = a / b;
        return true;
    }

    return false;
}

static inline int32_t safe_sub_int32_int32(int32_t a, int32_t b)
{
    int64_t tmp = (int64_t)a - (int64_t)b;
    return safe_cast_int32_int64(tmp);
}

static inline bool check_sub_int32_int32(int32_t a, int32_t b, int32_t* ret)
{
    int64_t tmp = (int64_t)a - (int64_t)b;
    *ret = (int32_t)tmp;
    return check_cast_int32_int64(tmp) == 0;
}

static inline int32_t safe_sub_int32_uint32(int32_t a, uint32_t b)
{
    int64_t tmp = (int64_t)a - (int64_t)b;
    return safe_cast_int32_int64(tmp);
}

static inline bool check_sub_int32_uint32(int32_t a, uint32_t b, int32_t* ret)
{
    int64_t tmp = (int64_t)a - (int64_t)b;
    *ret = (int32_t)tmp;
    return check_cast_int32_int64(tmp) == 0;
}

static inline int32_t safe_sub_int32_int64(int32_t a, int64_t b)
{
    // We have 4 fairly complex cases:
    // lhs positive, rhs positive - rhs could be larger than lhs can represent
    // lhs positive, rhs negative - additive case - check tmp >= lhs and tmp > max int
    // lhs negative, rhs positive - check tmp <= lhs and tmp < min int
    // lhs negative, rhs negative - addition cannot internally overflow, check against max

    int64_t tmp = (int64_t)((uint64_t)a - (uint64_t)b);

    if (a >= 0)
    {
        // first case
        if (b >= 0)
        {
            if (tmp >= INT32_MIN)
            {
                return (int32_t)tmp;
            }
        }
        else
        {
            // second case
            if (tmp >= a && tmp <= INT32_MAX)
            {
                return (int32_t)tmp;
            }
        }
    }
    else
    {
        // lhs < 0
        // third case
        if (b >= 0)
        {
            if (tmp <= a && tmp >= INT32_MIN)
            {
                return (int32_t)tmp;
            }
        }
        else
        {
            // fourth case
            if (tmp <= INT32_MAX)
            {
                return (int32_t)tmp;
            }
        }
    }

    safe_math_fail("safe_math_fail safe_sub_int32_int64");
}

static inline bool check_sub_int32_int64(int32_t a, int64_t b, int32_t* ret)
{
    // See above for documentation
    int64_t tmp = (int64_t)((uint64_t)a - (uint64_t)b);

    if (a >= 0)
    {
        // first case
        if (b >= 0)
        {
            if (tmp >= INT32_MIN)
            {
                *ret = (int32_t)tmp;
                return true;
            }
        }
        else
        {
            // second case
            if (tmp >= a && tmp <= INT32_MAX)
            {
                *ret = (int32_t)tmp;
                return true;
            }
        }
    }
    else
    {
        // lhs < 0
        // third case
        if (b >= 0)
        {
            if (tmp <= a && tmp >= INT32_MIN)
            {
                *ret = (int32_t)tmp;
                return true;
            }
        }
        else
        {
            // fourth case
            if (tmp <= INT32_MAX)
            {
                *ret = (int32_t)tmp;
                return true;
            }
        }
    }

    return false;
}

static inline int32_t safe_sub_int32_uint64(int32_t a, uint64_t b)
{
    // We need the absolute value of INT32_MIN
    // This will give it to us without extraneous compiler warnings
    const uint64_t AbsMinInt32 = (uint64_t)INT32_MAX + 1;

    if (a < 0)
    {
        if (b <= AbsMinInt32 - safe_abs32(a))
        {
            return (int32_t)(a - (int64_t)b);
        }
    }
    else
    {
        if (b <= AbsMinInt32 + (uint64_t)a)
        {
            return (int32_t)(a - (int64_t)b);
        }
    }

    safe_math_fail("safe_math_fail safe_sub_int32_uint64");
}

static inline bool check_sub_int32_uint64(int32_t a, uint64_t b, int32_t* ret)
{
    // We need the absolute value of INT32_MIN
    // This will give it to us without extraneous compiler warnings
    const uint64_t AbsMinInt32 = (uint64_t)INT32_MAX + 1;

    if (a < 0)
    {
        if (b <= AbsMinInt32 - safe_abs32(a))
        {
            *ret = (int32_t)(a - (int64_t)b);
            return true;
        }
    }
    else
    {
        if (b <= AbsMinInt32 + (uint64_t)a)
        {
            *ret = (int32_t)((int64_t)a - (int64_t)b);
            return true;
        }
    }

    return false;
}

static inline uint32_t safe_sub_uint32_int32(uint32_t a, int32_t b)
{
    int64_t tmp = (int64_t)a - (int64_t)b;
    return safe_cast_uint32_int64(tmp);
}

static inline bool check_sub_uint32_int32(uint32_t a, int32_t b, uint32_t* ret)
{
    int64_t tmp = (int64_t)a - (int64_t)b;
    *ret = (uint32_t)tmp;
    return check_cast_uint32_int64(tmp) == 0;
}

static inline uint32_t safe_sub_uint32_uint32(uint32_t a, uint32_t b)
{
    if (a >= b)
        return a - b;

    safe_math_fail("safe_math_fail safe_sub_uint32_uint32");
}

static inline bool check_sub_uint32_uint32(uint32_t a, uint32_t b, uint32_t* ret)
{
    if (a >= b)
    {
        *ret = a - b;
        return true;
    }

    return false;
}

static inline uint32_t safe_sub_uint32_int64(uint32_t a, int64_t b)
{
    // must first see if rhs is positive or negative
    if (b >= 0)
    {
        if ((uint64_t)b <= a)
        {
            return (uint32_t)(a - (uint32_t)b);
        }
    }
    else
    {
        // we're now effectively adding
        // since lhs is 32-bit, and rhs cannot exceed 2^63
        // this addition cannot overflow
        uint64_t tmp = a + (uint64_t)negate64(b); // negation safe

        // but we could exceed UINT32_MAX
        if (tmp <= UINT32_MAX)
        {
            return (uint32_t)tmp;
        }
    }

    safe_math_fail("safe_math_fail safe_sub_uint32_int64");
}

static inline bool check_sub_uint32_int64(uint32_t a, int64_t b, uint32_t* ret)
{
    // must first see if rhs is positive or negative
    if (b >= 0)
    {
        if ((uint64_t)b <= a)
        {
            *ret = (uint32_t)(a - (uint32_t)b);
            return true;
        }
    }
    else
    {
        // we're now effectively adding
        // since lhs is 32-bit, and rhs cannot exceed 2^63
        // this addition cannot overflow
        uint64_t tmp = a + (uint64_t)negate64(b); // negation safe

        // but we could exceed UINT32_MAX
        if (tmp <= UINT32_MAX)
        {
            *ret = (uint32_t)tmp;
            return true;
        }
    }

    return false;
}

static inline uint32_t safe_sub_uint32_uint64(uint32_t a, uint64_t b)
{
    if (a >= b)
        return (uint32_t)(a - b);

    safe_math_fail("safe_math_fail safe_sub_uint32_uint64");
}

static inline bool check_sub_uint32_uint64(uint32_t a, uint64_t b, uint32_t* ret)
{
    if (a >= b)
    {
        *ret = (uint32_t)(a - b);
        return true;
    }
    return false;
}

static inline int64_t safe_sub_int64_int32(int64_t a, int32_t b)
{
    // we have essentially 4 cases:
    //
    // 1) lhs positive, rhs positive - overflow not possible
    // 2) lhs positive, rhs negative - equivalent to addition - result >= lhs or error
    // 3) lhs negative, rhs positive - check result <= lhs
    // 4) lhs negative, rhs negative - overflow not possible

    int64_t tmp = (int64_t)((uint64_t)a - (uint64_t)b);

    // Note - ideally, we can order these so that true conditionals
    // lead to success, which enables better pipelining
    // It isn't practical here
    if ((a >= 0 && b < 0 && tmp < a) || // condition 2
        (b >= 0 && tmp > a))              // condition 3
    {
        safe_math_fail("safe_math_fail safe_sub_int64_int32");
    }

    return tmp;
}

static inline bool check_sub_int64_int32(int64_t a, int32_t b, int64_t* ret)
{
    int64_t tmp = (int64_t)((uint64_t)a - (uint64_t)b);

    // Note - ideally, we can order these so that true conditionals
    // lead to success, which enables better pipelining
    // It isn't practical here
    if ((a >= 0 && b < 0 && tmp < a) || // condition 2
        (b >= 0 && tmp > a))              // condition 3
    {
        return false;
    }

    *ret = tmp;
    return true;
}

static inline int64_t safe_sub_int64_uint32(int64_t a, uint32_t b)
{
    // lhs is a 64-bit int, rhs unsigned int32 or smaller
    // perform test as unsigned to prevent unwanted optimizations
    uint64_t tmp = (uint64_t)a - (uint64_t)b;

    if ((int64_t)tmp <= a)
    {
        return (int64_t)tmp;
    }

    safe_math_fail("safe_math_fail safe_sub_int64_int64");
}

static inline bool check_sub_int64_uint32(int64_t a, uint32_t b, int64_t* ret)
{
    uint64_t tmp = (uint64_t)a - (uint64_t)b;

    if ((int64_t)tmp <= a)
    {
        *ret = (int64_t)tmp;
        return true;
    }

    return false;
}

static inline int64_t safe_sub_int64_int64(int64_t a, int64_t b)
{
    // we have essentially 4 cases:
    //
    // 1) lhs positive, rhs positive - overflow not possible
    // 2) lhs positive, rhs negative - equivalent to addition - result >= lhs or error
    // 3) lhs negative, rhs positive - check result <= lhs
    // 4) lhs negative, rhs negative - overflow not possible

    int64_t tmp = (int64_t)((uint64_t)a - (uint64_t)b);

    // Note - ideally, we can order these so that true conditionals
    // lead to success, which enables better pipelining
    // It isn't practical here
    if ((a >= 0 && b < 0 && tmp < a) || // condition 2
        (b >= 0 && tmp > a))              // condition 3
    {
        safe_math_fail("safe_math_fail safe_sub_int64_int64");
    }

    return tmp;
}

static inline bool check_sub_int64_int64(int64_t a, int64_t b, int64_t* ret)
{
    int64_t tmp = (int64_t)((uint64_t)a - (uint64_t)b);

    // Note - ideally, we can order these so that true conditionals
    // lead to success, which enables better pipelining
    // It isn't practical here
    if ((a >= 0 && b < 0 && tmp < a) || // condition 2
        (b >= 0 && tmp > a))              // condition 3
    {
        return false;
    }

    *ret = tmp;
    return true;
}

static inline int64_t safe_sub_int64_uint64(int64_t a, uint64_t b)
{
    // if we subtract, and it gets larger, there's a problem
    // Perform test as unsigned to prevent unwanted optimizations
    uint64_t tmp = (uint64_t)a - b;

    if ((int64_t)tmp <= a)
    {
        return (int64_t)tmp;
    }

    safe_math_fail("safe_math_fail safe_sub_int64_uint64");
}

static inline bool check_sub_int64_uint64(int64_t a, uint64_t b, int64_t* ret)
{
    uint64_t tmp = (uint64_t)a - b;
    *ret = (int64_t)tmp;

    return ((int64_t)tmp <= a);
}

static inline uint64_t safe_sub_uint64_int32(uint64_t a, int32_t b)
{
    // lhs is an uint64_t, rhs signed
    // must first see if rhs is positive or negative
    if (b >= 0)
    {
        if ((uint64_t)b <= a)
        {
            return (uint64_t)(a - (uint64_t)b);
        }
    }
    else
    {
        uint64_t tmp = a;
        // we're now effectively adding
        uint64_t result = a + safe_abs64(b);

        if (result >= tmp)
            return result;
    }

    safe_math_fail("safe_math_fail safe_sub_uint64_int32");
}

static inline bool check_sub_uint64_int32(uint64_t a, int32_t b, uint64_t* ret)
{
    if (b >= 0)
    {
        if ((uint64_t)b <= a)
        {
            *ret = (uint64_t)(a - (uint64_t)b);
            return true;
        }
    }
    else
    {
        uint64_t tmp = a;
        // we're now effectively adding
        uint64_t result = a + safe_abs64(b);

        if (result >= tmp)
        {
            *ret = result;
            return true;
        }
    }

    return false;
}

static inline uint64_t safe_sub_uint64_uint32(uint64_t a, uint32_t b)
{
    uint64_t tmp = a - b;

    if (tmp <= a)
        return tmp;

    safe_math_fail("safe_math_fail safe_sub_uint64_uint32");
}

static inline bool check_sub_uint64_uint32(uint64_t a, uint32_t b, uint64_t* ret)
{
    uint64_t tmp = a - b;
    *ret = tmp;
    return (tmp <= a);
}

static inline uint64_t safe_sub_uint64_int64(uint64_t a, int64_t b)
{
    uint64_t result = 0;

    // must first see if rhs is positive or negative
    if (b >= 0)
    {
        if ((uint64_t)b <= a)
        {
            return (a - (uint64_t)b);
        }
    }
    else
    {
        // we're now effectively adding
        result = a + safe_abs64(b);

        if (result >= a)
            return result;
    }

    safe_math_fail("safe_math_fail safe_sub_uint64_int64");
}

static inline bool check_sub_uint64_int64(uint64_t a, int64_t b, uint64_t* ret)
{
    uint64_t result = 0;

    // must first see if rhs is positive or negative
    if (b >= 0)
    {
        if ((uint64_t)b <= a)
        {
            *ret = (a - (uint64_t)b);
            return true;
        }
    }
    else
    {
        // we're now effectively adding
        result = a + safe_abs64(b);

        if (result >= a)
        {
            *ret = result;
            return true;
        }
    }

    return false;
}

static inline uint64_t safe_sub_uint64_uint64(uint64_t a, uint64_t b)
{
    uint64_t tmp = a - b;

    if (tmp <= a)
        return tmp;

    safe_math_fail("safe_math_fail safe_sub_uint64_uint64");
}

static inline bool check_sub_uint64_uint64(uint64_t a, uint64_t b, uint64_t* ret)
{
    uint64_t tmp = a - b;
    *ret = tmp;
    return (tmp <= a);
}

#ifdef __cplusplus
} 
#endif

#endif // C_SAFE_MATH_IMPL


#define MAX(x, y) ((x) > (y) ? (x) : (y))

#if UNIT_MAX >> 32 == 0
#    define check_add_int_int check_add_int32_int32
#    define check_add_uint_uint check_add_uint32_uint32
#    define check_mul_int_int check_mul_int32_int32
#    define check_mul_uint_uint check_mul_uint32_uint32
#else
#    if UNIT_MAX >> 64 == 0
#        define check_add_int_int check_add_int64_int64
#        define check_add_uint_uint check_add_uint64_uint64
#        define check_mul_int_int check_mul_int64_int64
#        define check_mul_uint_uint check_mul_uint64_uint64
#    else
#        error unsupported integer size
#    endif
#endif

#ifndef BIGINT_REDZONE
#define BIGINT_REDZONE 0
#endif//BIGINT_REDZONE

#if BIGINT_REDZONE
// if BIGINT_REDZONE is set to a value, that value is the number of bytes
// of extra allocation at the front and the back of the digits buffer.
// those "redzones" will then be filled with an uncommon value (0x42)
// when freed, those "redzones" will be checked to make sure they weren't modified
unsigned char* malloc_digits(unsigned int num_digits) {
    unsigned int bytes;
    if(
        !check_mul_uint_uint(num_digits, sizeof(unsigned char), &bytes)
        || !check_add_uint_uint(bytes, BIGINT_REDZONE * 2, &bytes)
    ) {
        errno = ENOMEM;
        return NULL;
    }
    unsigned char* p = malloc(bytes);
    memset(p, 0x42, bytes);
    return p + BIGINT_REDZONE;
}

unsigned char* okay_digits(unsigned char* digits, unsigned int num_digits) {
    unsigned char* rz1 = digits - BIGINT_REDZONE;
    unsigned char* rz2 = digits + num_digits * sizeof(unsigned char);
    for(unsigned int i = 0; i < BIGINT_REDZONE; i++) {
        if(rz1[i] != 0x42) {
            fprintf(stderr, "redzone underflow\n");
            return NULL;
        }
        if(rz2[i] != 0x42) {
            fprintf(stderr, "redzone overflow\n");
            return NULL;
        }
    }
    return rz1;
}

void free_digits(unsigned char* digits, unsigned int num_digits) {
    if(!digits) {
        return;
    }
    unsigned char* p = okay_digits(digits, num_digits);
    assert(p); // redzone violation
    free(p);
}
#else
#define malloc_digits(num_digits) malloc((num_digits) * sizeof(unsigned char))
#define okay_digits(digits,num_digits) 1
#define free_digits(digits,num_digits) free(digits)
#endif

BigInt* BigInt_construct(int value) {

    BigInt* new_big_int = malloc(sizeof(BigInt));
    if(!new_big_int) {
        return NULL;
    }
    unsigned int value2;
    if(value < 0) {
        new_big_int->is_negative = 1;
        value2 = -value;
    } else {
        new_big_int->is_negative = 0;
        value2 = value;
    }

    new_big_int->num_digits = floor(log10(value2)) + 1;

    // Special case for 0
    if(new_big_int->num_digits == 0) {
        new_big_int->num_digits = 1;
    }

    new_big_int->num_allocated_digits = new_big_int->num_digits;
    new_big_int->digits = malloc_digits(new_big_int->num_allocated_digits);
    if(!new_big_int->digits) {
        free(new_big_int);
        return NULL;
    }

    unsigned int count = new_big_int->num_digits;
    unsigned char* digits = new_big_int->digits;
    while(count--) {
        (*digits++) = value2 % 10;
        value2 /= 10;
    }

    return new_big_int;
}

BigInt* BigInt_clone(const BigInt* big_int, unsigned int num_allocated_digits) {
    if(num_allocated_digits < big_int->num_digits) {
        num_allocated_digits = big_int->num_digits;
    }
    BigInt* new_big_int = malloc(sizeof(BigInt));
    if(!new_big_int) {
        return NULL;
    }
    new_big_int->digits = malloc_digits(num_allocated_digits);
    if(!new_big_int->digits) {
        free(new_big_int);
        return NULL;
    }
    new_big_int->num_allocated_digits = num_allocated_digits;
    new_big_int->is_negative = big_int->is_negative;
    new_big_int->num_digits = big_int->num_digits;
    memmove(new_big_int->digits, big_int->digits, big_int->num_digits * sizeof(unsigned char));
    assert(okay_digits(new_big_int->digits, new_big_int->num_allocated_digits));
    return new_big_int;
}

BigInt* BigInt_from_string(const char* str) {
    BOOL is_negative = (*str == '-');
    if(is_negative) {
        str++;
    }
    while(*str == '0' && *str != 0) { // remove leading zeros
        str++;
    }
    unsigned int num_digits = strlen( str );
    BigInt* new_big_int = malloc(sizeof(BigInt));
    if(!new_big_int){
        return NULL;
    }
    new_big_int->is_negative = is_negative;
    new_big_int->num_allocated_digits = num_digits;
    new_big_int->digits = malloc_digits(num_digits);
    if(!new_big_int->digits){
        free(new_big_int);
        return NULL;
    }
    const char* end = str + num_digits - 1;
    unsigned char* digits = new_big_int->digits;
    while( end >= str ){
        unsigned char digit = *(end--);
        if(digit < '0' || digit > '9'){
            BigInt_free(new_big_int);
            errno = EINVAL;
            return NULL;
        }
        *digits++ = digit - '0';
    }
    new_big_int->num_digits = digits - new_big_int->digits;
    assert(okay_digits(new_big_int->digits, new_big_int->num_allocated_digits));
    return new_big_int;
}

void BigInt_free(BigInt* big_int) {
    if(big_int) {
        free_digits(big_int->digits, big_int->num_allocated_digits);
        free(big_int);
    }
}

BOOL BigInt_assign(BigInt* target, const BigInt* source)
{
    if(!BigInt_ensure_digits(target, source->num_digits)) {
        return 0;
    }

    memmove(target->digits, source->digits, source->num_digits * sizeof(unsigned char));

    target->is_negative = source->is_negative;
    target->num_digits = source->num_digits;
    return 1;
}

BOOL BigInt_assign_int(BigInt* target, const int source) {
    unsigned int value;
    if(value < 0) {
        target->is_negative = 1;
        value = -source;
    } else {
        target->is_negative = 0;
        value = source;
    }

    target->num_digits = floor(log10(value)) + 1;

    // Special case for 0
    if(target->num_digits == 0) {
        target->num_digits = 1;
    }

    if(!BigInt_ensure_digits(target, target->num_digits)) {
        return 0;
    }

    unsigned int count = target->num_digits;
    unsigned char* digits = target->digits;
    while(count--) {
        *(digits++) = value % 10;
        value /= 10;
    }
    return 1;
}

int BigInt_compare(const BigInt* a, const BigInt* b) {
    // Quick return if one is negative and the other isn't
    if(a->num_digits > 0 || a->digits[0] > 0 || b->num_digits > 0 || b->digits[0] > 0) {
        if (a->is_negative && !b->is_negative) {
            return -1;
        } else if (!a->is_negative && b->is_negative) {
            return 1;
        }
    }

    return a->is_negative ? BigInt_compare_digits(b, a) : BigInt_compare_digits(a, b);
}

int BigInt_compare_int(const BigInt* a, int b) {
    int aa;
    if(!BigInt_to_int(a, &aa)) {
        // a is too big to fit into an integer:
        return a->is_negative ? -1 : 1;
    }
    if(aa == b) {
        return 0;
    }
    return aa < b ? -1 : 1;
}

int BigInt_compare_digits(const BigInt* a, const BigInt* b) {
    // Not looking at the sign here, comparing the digits only.

    // Quick return if one number has more digits than the other
    if(a->num_digits > b->num_digits) {
       return 1;
    } else if(a->num_digits < b->num_digits) {
       return -1;
    }

    // Both have the same number of digits, so we actually have to loop through until we
    // find one that doesn't match.
    unsigned int count = a->num_digits;
    const unsigned char* pa = &a->digits[count-1];
    const unsigned char* pb = &b->digits[count-1];
    while(count--) {
        char da = *(pa--);
        char db = *(pb--);
        if(da > db) {
            return 1;
        } else if(da < db) {
            return -1;
        }
    }

    // All digits match; numbers are equal
    return 0;
}

BOOL BigInt_add(BigInt* big_int, const BigInt* addend) {
    if(big_int->is_negative == addend->is_negative) {
        // Sign will never change in this case so just leave
        // it as-is.
        if(!BigInt_add_digits(big_int, addend)) {
            return 0;
        }
    } else {
        // Figure out the sign.  Need to do this before calculating the digits of
        // the digits result because changing those in big_int will affect the result
        // of the compare.
        unsigned int result_is_negative = BigInt_compare_digits(big_int, addend) > 0 ?
                big_int->is_negative : addend->is_negative;

        if(!BigInt_subtract_digits(big_int, addend)) {
            return 0;
        }
        big_int->is_negative = result_is_negative;
    }
    return 1;
}

BOOL BigInt_add_int(BigInt* big_int, const int addend) {
    BigInt* big_int_addend = BigInt_construct(addend);
    if(!big_int_addend) {
        return 0;
    }
    BOOL result = BigInt_add(big_int, big_int_addend);
    BigInt_free(big_int_addend);
    return result;
}

BOOL BigInt_add_digits(BigInt* big_int, const BigInt* addend) {
    unsigned int digits_needed = MAX(big_int->num_digits, addend->num_digits) + 1; // TODO FIXME: this can overflow...
    if(!BigInt_ensure_digits(big_int, digits_needed)) {
        return 0;
    }

    int i;
    int carry = 0;
    for(i = 0; i < addend->num_digits || carry > 0; ++i) { // TODO FIXME: refactor to protect from integer overflow
        // Append another digit if necessary
        if(i == big_int->num_digits) {
            ++big_int->num_digits;
            big_int->digits[i] = 0;
        }

        unsigned int addend_digit = i < addend->num_digits ? addend->digits[i] : 0;
        unsigned int total = big_int->digits[i] + addend_digit + carry;
        big_int->digits[i] = total % 10;
        carry = (total >= 10) ? 1 : 0;
    }
    return 1;
}

BOOL BigInt_subtract(BigInt* big_int, const BigInt* to_subtract) {
    // Figure out the sign.  Need to do this before calculating the digits of
    // the digits result because changing those in big_int will affect the result
    // of the compare.
    unsigned int result_is_negative = BigInt_compare(big_int, to_subtract) >= 0 ? 0 : 1;
    
    // Calculate the digits
    if(big_int->is_negative == to_subtract->is_negative) {
        if(!BigInt_subtract_digits(big_int, to_subtract)) {
            return 0;
        }
    } else {
        if(!BigInt_add_digits(big_int, to_subtract)) {
            return 0;
        }
    }
    
    // Figure out the sign
    big_int->is_negative = result_is_negative;
    return 1;
}


BOOL BigInt_subtract_int(BigInt* big_int, const int to_subtract) {
    BigInt* big_int_to_subtract = BigInt_construct(to_subtract);
    if(!big_int_to_subtract) {
        return 0;
    }
    BOOL result = BigInt_subtract(big_int, big_int_to_subtract);
    BigInt_free(big_int_to_subtract);
    return result;
}

BOOL BigInt_subtract_digits(BigInt* big_int, const BigInt* to_subtract) {

    unsigned int digits_needed = MAX(big_int->num_digits, to_subtract->num_digits) + 1;
    if(!BigInt_ensure_digits(big_int, digits_needed)) {
        return 0;
    }

    // Determine the larger int.  This will go on "top"
    // of the subtraction.  Sign doesn't matter here since we've already
    // determined the sign of the final result above.
    unsigned char* greater_int_digits;
    unsigned char* smaller_int_digits;
    unsigned int smaller_int_num_digits;
    unsigned int greater_int_num_digits;

    if(BigInt_compare_digits(big_int, to_subtract) > 0) {
        greater_int_digits = big_int->digits;
        greater_int_num_digits = big_int->num_digits;
        smaller_int_digits = to_subtract->digits;
        smaller_int_num_digits = to_subtract->num_digits;
    } else {
        greater_int_digits = to_subtract->digits;
        greater_int_num_digits = to_subtract->num_digits;
        smaller_int_digits = big_int->digits;
        smaller_int_num_digits = big_int->num_digits;
    }

    // Actually carry out the subtraction.
    int i;
    int carry = 0;
    big_int->num_digits = 1;

    for(i = 0; i < greater_int_num_digits; ++i) {
        int new_digit;
        if(i < smaller_int_num_digits) {
            new_digit = (int)greater_int_digits[i] - (int)smaller_int_digits[i] + carry;
        } else {
            new_digit = (int)greater_int_digits[i] + carry;
        }

        // Carry 10 from the next digit if necessary
        if(new_digit < 0) {
            carry = -1;
            new_digit += 10;
        } else {
            carry = 0;
        }

        assert(new_digit >= 0);
        big_int->digits[i] = new_digit;
        if(new_digit != 0) {
            big_int->num_digits = i + 1;
        }
    }

    assert(carry == 0);
    return 1;
}

// Multiply using the pencil and paper method.  Complexity is O(n*m) where n, m are
// the number of digits in big_int and multiplier, respectively.
BOOL BigInt_multiply(BigInt* big_int, const BigInt* multiplier) {

    // Need to keep track of the result in a separate variable because we need
    // big_int to retain its original value throughout the course of the calculation.
    BigInt* result = BigInt_construct(0);
    if(!result) {
        return 0;
    }

    // addend will hold the amount to be added to the result for each step of
    // the multiplication.
    BigInt* addend = BigInt_construct(0);
    if(!addend) {
        BigInt_free(result);
        return 0;
    }

    unsigned int digits_needed = big_int->num_digits + multiplier->num_digits + 1;
    if(!BigInt_ensure_digits(addend, digits_needed)) {
        BigInt_free(result);
        BigInt_free(addend);
        return 0;
    }

    int i, j;
    int carry = 0;
    for(i = 0; i < multiplier->num_digits; ++i) {

        if(i > 0) {
            addend->num_digits = i;
            addend->digits[i - 1] = 0;
        }

        for(j = 0; j < big_int->num_digits || carry > 0; ++j) { // TODO FIXME: potential infinite loop
            if(j + i == addend->num_digits) {
                ++addend->num_digits;
            }

            assert(digits_needed >= j + 1);

            int total;
            if(j < big_int->num_digits) {
                total = (big_int->digits[j] * multiplier->digits[i]) + carry;
            } else {
                total = carry;
            }

            addend->digits[i + j] = total % 10;
            carry = total / 10;
        }

        if(!BigInt_add(result, addend)) {
            BigInt_free(result);
            BigInt_free(addend);
            return 0;
        }
    }

    result->is_negative = big_int->is_negative != multiplier->is_negative;

    // don't leave 0's in highest digit
    while(result->num_digits > 1 && !result->digits[result->num_digits-1]) {
        result->num_digits--;
    }

    // Place the result in big_int and clean things up
    BOOL success = BigInt_assign(big_int, result);
    BigInt_free(result);
    BigInt_free(addend);
    return success;
}

BOOL BigInt_multiply_int(BigInt* big_int, const int multiplier) {
    BigInt* big_int_multiplier = BigInt_construct(multiplier);
    if(!big_int_multiplier) {
        return 0;
    }
    BOOL result = BigInt_multiply(big_int, big_int_multiplier);
    BigInt_free(big_int_multiplier);
    return result;
}

BOOL BigInt_divide(
    BigInt* dividend, BigInt* divisor,
    BigInt* quotient, BigInt* remainder)
{
    int result = 0; // default to failure
    BigInt* div2 = NULL;
    BigInt* div3 = NULL;
    BigInt* div4 = NULL;
    BigInt* div5 = NULL;
    BigInt* div6 = NULL;
    BigInt* div7 = NULL;
    BigInt* div8 = NULL;
    BigInt* div9 = NULL;
    BigInt* ten = NULL;
    BigInt* _quotient = NULL;
    BigInt* _remainder = NULL;

    if(!BigInt_compare_int(divisor, 0)) {
        errno = ERANGE; // even BigInt can't represent infinity
        goto cleanup;
    }
    div2 = BigInt_clone(divisor, divisor->num_digits + 1);
    if(!div2 || !BigInt_multiply_int(div2, 2)) {
        goto cleanup;
    }
    div3 = BigInt_clone(divisor, divisor->num_digits + 1);
    if(!div3 || !BigInt_multiply_int(div3, 3)) {
        goto cleanup;
    }
    div4 = BigInt_clone(divisor, divisor->num_digits + 1);
    if(!div4 || !BigInt_multiply_int(div4, 4)) {
        goto cleanup;
    }
    div5 = BigInt_clone(divisor, divisor->num_digits + 1);
    if(!div5 || !BigInt_multiply_int(div5, 5)) {
        goto cleanup;
    }
    div6 = BigInt_clone(divisor, divisor->num_digits + 1);
    if(!div6 || !BigInt_multiply_int(div6, 6)) {
        goto cleanup;
    }
    div7 = BigInt_clone(divisor, divisor->num_digits + 1);
    if(!div7 || !BigInt_multiply_int(div7, 7)) {
        goto cleanup;
    }
    div8 = BigInt_clone(divisor, divisor->num_digits + 1);
    if(!div8 || !BigInt_multiply_int(div8, 8)) {
        goto cleanup;
    }
    div9 = BigInt_clone(divisor, divisor->num_digits + 1);
    if(!div9 || !BigInt_multiply_int(div9, 9)) {
        goto cleanup;
    }

    ten = BigInt_construct( 10 );
    _quotient = BigInt_construct( 0 );
    _remainder = BigInt_construct( 0 );
    if(!ten || !_quotient || !_remainder) {
        goto cleanup;
    }

    BigInt* divs[10];
    // NOTE: divs[0] intentionally unused so that new_digit == index below
    divs[1] = divisor;
    divs[2] = div2;
    divs[3] = div3;
    divs[4] = div4;
    divs[5] = div5;
    divs[6] = div6;
    divs[7] = div7;
    divs[8] = div8;
    divs[9] = div9;

    const unsigned char* base = dividend->digits;
    const unsigned char* digits = &base[dividend->num_digits-1];

    while(digits >= base) {
        if(!BigInt_multiply(_remainder, ten)) {
            goto cleanup;
        }
        if(!BigInt_add_int(_remainder, *digits)) {
            goto cleanup;
        }
        int new_digit = 0;
        for(int i = 9; i >= 1; i--) {
            if(BigInt_compare(_remainder, divs[i]) >= 0) {
                if(!BigInt_subtract(_remainder, divs[i])) {
                    goto cleanup;
                }
                new_digit = i;
                break;
            }
        }
        if(!BigInt_multiply(_quotient, ten) || !BigInt_add_int(_quotient, new_digit)) {
            goto cleanup;
        }
        digits--;
    }
    
    if(quotient) {
        if(!BigInt_assign(quotient, _quotient)) {
            goto cleanup;
        }
    }
    if(remainder) {
        if(!BigInt_assign(remainder, _remainder)) {
            goto cleanup;
        }
    }
    
    result = 1;
cleanup:
    BigInt_free(div2);
    BigInt_free(div3);
    BigInt_free(div4);
    BigInt_free(div5);
    BigInt_free(div6);
    BigInt_free(div7);
    BigInt_free(div8);
    BigInt_free(div9);
    BigInt_free(ten);
    BigInt_free(_remainder);
    BigInt_free(_quotient);
    return result;
}

BOOL BigInt_to_int(const BigInt* big_int, int* value) {
    *value = 0;
    int tens_multiplier = 1;

    unsigned int num_digits = big_int->num_digits;
    const unsigned char* digits = big_int->digits;
    
    // don't process any most significant digits that happen to be zero
    // (avoids unnecessary tens_multiplier overflow)
    while(num_digits && !digits[num_digits-1]) {
        num_digits--;
    }

    // prefill value so that we can avoid an unnecessary tens_multiplier overflow
    if(num_digits) {
        *value = *(digits++);
        num_digits--;
    }
    while(num_digits--) {
        int digit = *(digits++);
        if(
            !check_mul_int_int(tens_multiplier, 10, &tens_multiplier)
            || !check_mul_int_int(digit, tens_multiplier, &digit)
            || !check_add_int_int(*value, digit, value)
        ) {
            errno = ERANGE;
            return 0;
        }
    }

    if (big_int->is_negative) {
        if(!check_mul_int_int(*value, -1, value)) {
            errno = ERANGE;
            return 0;
        }
    }

    return 1;

}

void BigInt_print(const BigInt* big_int) {
    BigInt_fprint(stdout, big_int);
}

void BigInt_fprint(FILE *dest, const BigInt* big_int) {
    const unsigned char* base = big_int->digits;
    const unsigned char* digits = &base[big_int->num_digits-1];
    if (big_int->is_negative) fputc('-', dest);
    while(digits >= base) {
        fputc('0' + *(digits--), dest);
    }
}

unsigned int BigInt_strlen(const BigInt* big_int){
    unsigned int len = big_int->num_digits;
    if( big_int->is_negative ){
        len++;
    }
    return len;
}

BOOL BigInt_to_string(const BigInt* big_int, char* buf, unsigned int buf_size){
    const unsigned char* base = big_int->digits;
    const unsigned char* digits = &base[big_int->num_digits-1];
    if (big_int->is_negative){
        if(!buf_size--){
            errno = ERANGE;
            return 0;
        }
        *buf++ = '-';
    }

    while( digits >= base ){
        if(!buf_size--){
            errno = ERANGE;
            return 0;
        }
        *buf++ = '0' + *(digits--);
    }

    // write 0 terminator:
    if(!buf_size--){
        errno = ERANGE;
        return 0;
    }
    *buf++ = 0;

    return 1;
}

char* BigInt_to_new_string(const BigInt* big_int){
    unsigned int buf_size = BigInt_strlen(big_int) + 1;
    char* buf = malloc(buf_size);
    if(!buf) {
        return NULL;
    }
    BOOL result = BigInt_to_string(big_int, buf, buf_size);
    assert(result);
    return buf;
}

BOOL BigInt_ensure_digits(BigInt* big_int, unsigned int digits_needed) {
    if(big_int->num_allocated_digits < digits_needed) {
        assert(okay_digits(big_int->digits, big_int->num_allocated_digits));
        unsigned char* new_digits = malloc_digits(digits_needed);
        if(!new_digits) {
            return 0;
        }
        assert(okay_digits(new_digits, digits_needed));
        unsigned int old_allocated = big_int->num_allocated_digits;
        unsigned char* old_digits = big_int->digits;
        memcpy(new_digits, old_digits, big_int->num_digits * sizeof(unsigned char));
        big_int->digits = new_digits;
        big_int->num_allocated_digits = digits_needed;

        free_digits(old_digits, old_allocated);
        assert(okay_digits(big_int->digits, big_int->num_allocated_digits));
    }
    return 1;
}
