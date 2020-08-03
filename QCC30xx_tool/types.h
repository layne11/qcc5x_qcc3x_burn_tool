/**********************************************************************
*
* types.h
*
* Copyright (c) 2010-2017 Qualcomm Technologies International, Ltd.
* All Rights Reserved.
* Qualcomm Technologies International, Ltd. Confidential and Proprietary.
*
* Type definitions.
*
**********************************************************************/
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H


#define HOSTTOOLS_OLD_INTEGER_TYPES

#ifndef HOSTTOOLS_OLD_INTEGER_TYPES

#include "common/types_t.h"

/*
 *  64 bit data model is explained below
 *
 * Datatype  LP64 ILP64 LLP64 ILP32 LP32 16bit
 * char      8    8     8     8     8    8
 * short     16   16    16    16    16   16
 * int       32   64    32    32    16   16
 * long      64   64    32    32    32   32
 * long long 64         64    64
 * pointer   64   64    64    32    32   32
 *
 * */

/*
 * Define (u)int<n> types to be identical to their (u)int<n>_t counterparts.
 */

typedef uint8_t  uint8;
typedef int8_t   int8;
typedef uint16_t uint16;
typedef int16_t  int16;
typedef uint32_t uint32;
typedef int32_t  int32;
typedef uint64_t uint64;
typedef int64_t  int64;
typedef uint32   uint24;           
typedef int32    int24;

typedef double   float64;

typedef void*    CsrHandle_t;

#ifdef __linux__
typedef int SOCKET;
#endif

typedef struct
{   
    /* will fail to compile if any of the sizes are not as expected
       the line it fails on is the type that has the wrong size */

#define FAIL_IF_SIZE_DOES_NOT_MATCH(x) ((x) ? 1 : -1)
    union
    {
        char bad_int8   [FAIL_IF_SIZE_DOES_NOT_MATCH(sizeof(int8)   == 1)];
        char bad_uint8  [FAIL_IF_SIZE_DOES_NOT_MATCH(sizeof(uint8)  == 1)];
        char bad_int16  [FAIL_IF_SIZE_DOES_NOT_MATCH(sizeof(int16)  == 2)];
        char bad_uint16 [FAIL_IF_SIZE_DOES_NOT_MATCH(sizeof(uint16) == 2)];
        char bad_int32  [FAIL_IF_SIZE_DOES_NOT_MATCH(sizeof(int32)  == 4)];
        char bad_uint32 [FAIL_IF_SIZE_DOES_NOT_MATCH(sizeof(uint32) == 4)];
    };
} CheckCompilerDataSizesAndFailIfInvalid;

#else /* HOSTTOOLS_OLD_INTEGER_TYPES */
 
#ifdef __linux__
#include <sys/types.h>
#endif

typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned long uint32;
typedef signed long int32;
/*
 * [u]int24 is defined as [u]int32. This is the only data type which is not
 * "exactly the size" that it represents.
 */

typedef unsigned long long uint64;
typedef signed long long int64;
typedef uint32 uint24;           
typedef int32 int24;

typedef double float64;

typedef void* CsrHandle_t;

#ifdef __linux__
typedef int SOCKET;
#endif

#endif /* HOSTTOOLS_OLD_INTEGER_TYPES */

#endif /* COMMON_TYPES_H */
