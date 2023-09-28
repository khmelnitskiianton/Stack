#ifndef HEAD_H
#define HEAD_H

#define ASSERT_STACK 1
#define ASSERT_CANARY 1
#define DEBUG_PRINTING 1

#if (ASSERT_STACK != 0)
#define ASSERT_STACK(stk, func)                             \                                                                        
        if (checkStack(stk, func) != 0)                     \
        {                                                   \
            STACK_DUMP (stk, checkStack(stk, func))         \
        }                                                     
#define STACK_DUMP(stk, error_code) output_error(stk, __FILE__, __LINE__, __PRETTY_FUNCTION__, error_code);
#else   
#define ASSERT_STACK(stk, func)     ;         \     
#define STACK_DUMP(stk, error_code) ;       
#endif  

typedef int Elem_t;
#if (ASSERT_CANARY != 0)
    typedef unsigned long long Canary_t;  
#endif

typedef struct Stack {
#if (ASSERT_CANARY != 0)
    Canary_t  l_canary;
#endif
    Elem_t    *data;
    int       size;
    int       capacity; 
#if (ASSERT_CANARY != 0)
    Canary_t  *l_canary_data;
    Canary_t  *r_canary_data;
    Canary_t  r_canary;
#endif
} Stack_t;

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h> 
#include <string.h>

#include "stack.h"

const Elem_t NULL_ELEMENT = NULL;
const Elem_t POISON_ELEMENT = INT_MAX;
const int    N_ERRORS = 10;
const int    MULTIPLIER = 2;
const int    MIN_LEN = 5;

#define MYDEADBABY 3735927486 
#define MYDEADBEEF 3735928559

#endif 