#ifndef HEAD_H
#define HEAD_H

#define ASSERT_STACK   1
#define ASSERT_CANARY  1
#define ASSERT_HASH    1
#define DEBUG_PRINTING 1

#if (ASSERT_STACK != 0)
#define ASSERT_STACK(stk, func)                             \                                                                        
        if (checkStack(stk, func) != 0)                     \
        {                                                   \
            STACK_DUMP (stk, checkStack(stk, func))         \
        }                                                     
#define STACK_DUMP(stk, error_code) output_error(stk, __FILE__, __LINE__, __PRETTY_FUNCTION__, error_code);
#else   
#define ASSERT_STACK(stk, func)                   
#define STACK_DUMP(stk, error_code)        
#endif

#if (ASSERT_CANARY != 0)
#define ON_CANARY(...)  __VA_ARGS__
#else
#define ON_CANARY(...)  
#endif

#if (ASSERT_CANARY != 0)
#define ON_CANARY_IF(...)  __VA_ARGS__
#define ON_CANARY_ELSE(...)  
#else
#define ON_CANARY_IF(...)
#define ON_CANARY_ELSE(...)  __VA_ARGS__
#endif

#if (ASSERT_HASH != 0)
#define ON_HASH(...) __VA_ARGS__
#else
#define ON_HASH(...) 
#endif

#if (DEBUG_PRINTING != 0)
#define ON_PRINTING(...) __VA_ARGS__
#else
#define ON_PRINTING(...) 
#endif

typedef int Elem_t;
ON_CANARY( 
typedef unsigned long long Canary_t; 
)
ON_HASH(
typedef unsigned int Hash_t;
)

typedef struct Stack {
ON_CANARY(
    Canary_t  l_canary;
    )
    Elem_t    *data;
    int       size;
    int       capacity; 

ON_HASH(
    Hash_t    hash_data;
    Hash_t    hash_struct;
)

ON_CANARY (
    Canary_t  *l_canary_data;
    Canary_t  *r_canary_data;
    Canary_t  r_canary;
    )
} Stack_t;

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h> 
#include <string.h>

#include "stack.h"

const Elem_t NULL_ELEMENT = NULL;
const Elem_t POISON_ELEMENT = INT_MAX;
const int    N_ERRORS = 12;
const int    MULTIPLIER = 2;
const int    MIN_LEN = 5;

#endif 