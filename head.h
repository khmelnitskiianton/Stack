#ifndef HEAD_H
#define HEAD_H

typedef int Elem_t;

typedef struct Stack {
    Elem_t    *data;
    int       size;
    int       capacity; 
} Stack_t;

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h> 

#include "stack.h"

const Elem_t NULL_ELEMENT = NULL;
const Elem_t POISON_ELEMENT = INT_MIN;
const int    N_ERRORS = 6;
const int    MULTIPLIER = 2;
const int    MIN_LEN = 6;

#endif 