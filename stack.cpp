#include "head.h"

int stackCtor (Stack_t *stk)
{
    stk -> data = (Elem_t*) calloc (1, sizeof(Elem_t));
    stk -> size = 0;
    stk -> capacity = 1;
    return 1;
}

int push (Stack_t *stk, Elem_t value)
{ 
    int code = NULL;
    code = checkStack(stk);
    if (code != 0) STACK_DUMP (stk, code)

    if ((stk -> size) == (stk -> capacity))
    {
        (stk -> data) = expansion (stk);
    }

    *((stk -> data) + (stk -> size)) = value;
    (stk -> size)++; 
    return 1;
}

Elem_t pop (Stack_t *stk)
{
    int code = NULL;
    code = checkStack(stk);
    if ((stk -> size) <= 0) code += 32;
    if (code != 0) STACK_DUMP (stk, code)

    Elem_t save_value = NULL_ELEMENT;
    (stk -> size)--;
    save_value = *((stk -> data) + (stk -> size));  
    if (((stk -> size) != 0) && ((stk -> size)*MULTIPLIER <= (stk -> capacity)) && ((stk -> capacity) >= MIN_LEN))
    {
        (stk -> data) = comprassion (stk);
    }
    *((stk -> data) + (stk -> size)) = POISON_ELEMENT;
    return save_value;
}

Elem_t* expansion (Stack_t *stk)
{
    (stk -> capacity) = (stk -> capacity) * MULTIPLIER;
    return (Elem_t*) realloc ((stk -> data), sizeof(Elem_t)*(stk -> capacity));
}

Elem_t* comprassion (Stack_t *stk)
{
    (stk -> capacity) = (stk -> capacity) / MULTIPLIER;
    return (Elem_t*) realloc ((stk -> data), sizeof(Elem_t)*(stk -> capacity));
}

int StackDtor (Stack_t *stk)
{
    free(stk -> data);
    return 1;
}

int checkStack (Stack_t *stk)
{
    int error_code = 0;
    
    if (!stk)                              error_code += 1;
    if (!(stk -> data))                    error_code += 2;
    if ((stk -> size) > (stk -> capacity)) error_code += 4;
    if ((stk -> size) < 0)                 error_code += 8;
    if ((stk -> capacity) <= 0)            error_code += 16;

    return error_code;
}

int output_error (Stack_t *stk, const char* file, const size_t line, const char* pretty_function, int error_code)
{
    int array[N_ERRORS] = {};
    int element = 0;
    int z = error_code;

    while (z > 0)
    {
        if (z % 2) array[element] = 1;
        z = z / 2;
        element++;
    }

    printf("\n\n");
    printf ("Stack[%p] called from %s (string: %d) in function %s\n"
    //      "\t\t%s          \n"
            "{             \n"
            "\tsize     = %d \n"
            "\tcapacity = %d \n"
            "\tdata[%p]      \n"
            "\t{             \n", stk, file, line, pretty_function, stk -> size, stk -> capacity, stk -> data);

    for (size_t i = 0; i < (stk -> capacity); i++)
    {
        if  (*((stk -> data) + i) != INT_MIN) printf("\t +++[%d] = %d\n", i, *((stk -> data) + i));
        else                                  printf("\t ???[%d] = %d (POISON)\n", i, *((stk -> data) + i));
    }

    printf ("\t} \n"
            "}   \n");

    printf("CODE ERROR: ");
    for (int i = 0; i < N_ERRORS; i++)
    {
        printf("%d", array[i]);
    }
    printf("\nDECODING: \n");


    if (array[0] == 1) printf("1: %s\n", "ADDRESS OF STRUCTURE == NULL");
    else printf("0: OK\n");
    if (array[1] == 1) printf("1: %s\n", "ADDRESS OF ARRAY IN STRUCTURE == NULL");
    else printf("0: OK\n");
    if (array[2] == 1) printf("1: %s\n", "SIZE > CAPACITY");
    else printf("0: OK\n");
    if (array[3] == 1) printf("1: %s\n", "SIZE < 0");
    else printf("0: OK\n");
    if (array[4] == 1) printf("1: %s\n", "CAPACITY <= 0");
    else printf("0: OK\n");
    if (array[5] == 1) printf("1: %s\n", "POP() BUT DATA EMPTY");
    else printf("0: OK\n");
    printf("\n\n");
    assert(0);
}

/*
1. stk == NULL
2. stk -> data == NULL
3. stk -> size <= stk -> capacity
4. stk -> size >= 0 ? 
5. stk -> capacity > 0
6.
*/