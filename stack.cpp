#include "head.h"

int stackCtor (Stack_t *stk)
{
    stk -> size = 0; 
    stk -> capacity = 1;

#if (ASSERT_CANARY != 0)                                                
    stk -> l_canary = MYDEADBABY;                                                                                                                   \
    stk -> l_canary_data = (Canary_t*) calloc (1, sizeof(Canary_t)*2 + sizeof(Elem_t) + ((stk -> capacity)*sizeof(Elem_t)) % sizeof(Canary_t));     \
    stk -> data = (Elem_t*) ((stk -> l_canary_data) + 1);                                                                                           \
    ptr_r_canary_data (stk);                                                                                                                        \
    *(stk -> l_canary_data) = MYDEADBEEF;                                                                                                           \
    *(stk -> r_canary_data) = MYDEADBEEF;                                                                                                           \
    stk -> r_canary = MYDEADBABY;                                       
#else
    stk -> data = (Elem_t*) calloc (1, sizeof(Elem_t));     
#endif
    return 1;
}

int push (Stack_t *stk, Elem_t value)
{ 
    ASSERT_STACK(stk, "push")

    if ((stk -> size) == (stk -> capacity))
    {
        (stk -> data) = expansion (stk);
    }

    *((stk -> data) + (stk -> size)) = value;
    (stk -> size)++; 

    ASSERT_STACK(stk, "push")

#if (DEBUG_PRINTING != 0)
    printing_stack (stk, "push");
#endif

    return 1;
}

int pop (Stack_t *stk, Elem_t* ret_value)
{
    ASSERT_STACK(stk, "pop")

    Elem_t save_value = NULL_ELEMENT;
    (stk -> size)--;
    save_value = *((stk -> data) + (stk -> size));  
    if (((stk -> size) != 0) && ((stk -> size)*MULTIPLIER < (stk -> capacity)) && ((stk -> capacity) > MIN_LEN))
    {
        (stk -> data) = comprassion (stk);
    }
    *((stk -> data) + (stk -> size)) = POISON_ELEMENT;
    *ret_value = save_value;

    ASSERT_STACK(stk, "pop")
    
#if (DEBUG_PRINTING != 0)
    printing_stack (stk, "pop");
#endif

    return 1;
}

Elem_t* expansion (Stack_t *stk)
{
#if (ASSERT_CANARY != 0)
    (stk -> capacity) = (stk -> capacity) * MULTIPLIER;                                                                                                                                     \
    stk -> l_canary_data = (Canary_t*) realloc ((stk -> l_canary_data), sizeof(Canary_t)*2 + sizeof(Elem_t)*(stk -> capacity) + ((stk -> capacity)*sizeof(Elem_t)) % sizeof(Canary_t));     \
    void* new_place = nullptr;                                                                                                                                                              \
    new_place = (void*)((stk -> l_canary_data) + 1);                                                                                                                                        \
    mem_poison (new_place + sizeof(Elem_t)*((stk -> size)+1), (sizeof(Elem_t))*((stk -> capacity)-(stk -> size)-1));                                                                        \
    ptr_r_canary_data (stk);                                                                                                                                                                \
    *(stk -> r_canary_data) = MYDEADBEEF;                                                                                                                                                   \
    return (Elem_t*) new_place;
#else
    (stk -> capacity) = (stk -> capacity) * MULTIPLIER;                                                                                      \
    Elem_t* new_place = (Elem_t*) realloc ((stk -> data), sizeof(Elem_t)*(stk -> capacity));                                                 \
    mem_poison (new_place + sizeof(Elem_t)*((stk -> size)+1), (sizeof(Elem_t))*((stk -> capacity)-(stk -> size)-1));                         \
    return new_place;
#endif
}

Elem_t* comprassion (Stack_t *stk)
{
#if (ASSERT_CANARY != 0)
    (stk -> capacity) = (stk -> capacity) / MULTIPLIER;
    stk -> l_canary_data = (Canary_t*) realloc ((stk -> l_canary_data), sizeof(Canary_t)*2 + sizeof(Elem_t)*(stk -> capacity) + ((stk -> capacity)*sizeof(Elem_t)) % sizeof(Canary_t));
    void* new_place = nullptr;
    new_place = (void*)((stk -> l_canary_data) + 1);
    ptr_r_canary_data (stk);
    *(stk -> r_canary_data) = MYDEADBEEF;
    return (Elem_t*) new_place;
#else
    (stk -> capacity) = (stk -> capacity) / MULTIPLIER;                             \
    return (Elem_t*) realloc ((stk -> data), sizeof(Elem_t)*(stk -> capacity));
#endif
}

int mem_poison (void* memptr, size_t num)
{
    memset (memptr, POISON_ELEMENT, num);    
    return 1;
}

int StackDtor (Stack_t *stk)
{
#if (ASSERT_CANARY != 0)
    free(stk -> l_canary_data);        
#else
    free(stk -> data);
#endif

    return 1;
}

int checkStack (Stack_t *stk, char* func)
{
    int error_code = 0;
    
    if (!stk)                                            error_code += 1;
    if (!(stk -> data))                                  error_code += 2;
    if ((stk -> size) > (stk -> capacity))               error_code += 4;
    if ((stk -> size) < 0)                               error_code += 8;
    if ((stk -> capacity) <= 0)                          error_code += 16;
#if (ASSERT_CANARY != 0)
    if ((stk -> l_canary) != MYDEADBABY)                 error_code += 32;    \
    if ((stk -> r_canary) != MYDEADBABY)                 error_code += 64;    \
    if (*(stk -> l_canary_data) != MYDEADBEEF)           error_code += 128;    \
    if (*(stk -> r_canary_data) != MYDEADBEEF)           error_code += 256;   
#endif
    if ((!strcmp(func, "pop"))&&((stk -> size) <= 0))    error_code += 512;

    return error_code;
}

int output_error (Stack_t *stk, const char* file, const size_t line, const char* pretty_function, int error_code)
{
    printf("\n\n");
    printf ("Stack[%p] called from %s (string: %d) in function %s\n"
    //      "\t\t%s          \n"
            "{               \n"
            "\tsize     = %d \n"
            "\tcapacity = %d \n"
            "\tdata[%p]      \n"
            "\t{             \n", stk, file, line, pretty_function, stk -> size, stk -> capacity, stk -> data);

    for (size_t i = 0; i < (stk -> capacity); i++)
    {
        if  (*((stk -> data) + i) != POISON_ELEMENT) printf("\t +++[%d] = %d\n", i, *((stk -> data) + i));
        else                                         printf("\t ---[%d] = %d (POISON)\n", i, *((stk -> data) + i));
    }

    printf ("\t} \n"
            "}   \n");

    const char* mass_of_errors[N_ERRORS] = {
        "ADDRESS OF STRUCTURE == NULL",
        "ADDRESS OF ARRAY IN STRUCTURE == NULL",
        "SIZE > CAPACITY",
        "SIZE < 0",
        "CAPACITY <= 0",
        "RIGHT CANARY IN STRUCT IS DEAD. F",
        "LEFT CANARY IN STRUCT IS DEAD. F",
        "RIGHT CANARY IN DATA IS DEAD. F",
        "LEFT CANARY IN DATA IS DEAD. F",
        "POP() BUT DATA EMPTY"
    };

    int z = error_code;
    int bin_error = 0;
    size_t element = 0;
    printf("\nERROR:\n");
    while (z > 0)
    {
        if (z % 2)
        {
            bin_error += degree(10, element);
            printf("1: %s\n", mass_of_errors[element]);
        }
        else
        {
            printf("0: OK\n");
        }
        z = z / 2;
        element++;
    }
    printf("ERROR CODE: %d\n", bin_error);
    printf("\n\n");
    abort();
    return 1;
}

int printing_stack (Stack* stk, char* name)
{
    printf("Action: %s\n", name);
    for (size_t i = 0; i < (stk -> capacity); i++)
    {
        printf("stack[%p] = %d\n", stk -> data + i, *((stk -> data) + i));
    }
    printf("Size: %d \t Capacity: %d\n", (stk -> size), (stk -> capacity));
    printf("\n\n");
    return 1;
}

int degree(int a, int b)
{
    int x = 1;
    for (int i = 0; i < b; i++)
    {
        x = x * a;
    }
    return x;
}
#if (ASSERT_CANARY != 0)
Canary_t* ptr_r_canary_data (Stack* stk)                                                                                                    \
{                                                                                                                                           \
    Canary_t* ptr = (Canary_t*)((char*)((stk -> data) + (stk -> capacity))+((stk -> capacity)*sizeof(Elem_t)) % sizeof(Canary_t));          \
    stk -> r_canary_data = ptr;                                                                                                             \
    return ptr;                                                                                                                             \
}   
#endif