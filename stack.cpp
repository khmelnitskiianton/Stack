#include "head.h"

int stackCtor (Stack_t *stk)
{
    stk -> size = 0; 
    stk -> capacity = 1;

ON_CANARY_IF(                                                
    stk -> l_canary = 0xDEADBABE;                                                                                                                   
    stk -> l_canary_data = (Canary_t*) calloc (1, size_data(stk));     
    stk -> data = (Elem_t*) ((stk -> l_canary_data) + 1);                                                                                           
    ptr_r_canary_data (stk);                                                                                                                        
    *(stk -> l_canary_data) = 0xDEADBEEF;                                                                                                           
    *(stk -> r_canary_data) = 0xDEADBEEF;                                                                                                           
    stk -> r_canary = 0xDEADBABE; 
ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data   = hash_func((const char*)(stk -> l_canary_data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)                                      
)
ON_CANARY_ELSE( 
    stk -> data = (Elem_t*) calloc (1, size_data(stk));     
ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)
)
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

ON_CANARY_IF(
ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> l_canary_data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)
)
ON_CANARY_ELSE(
ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)
)

    ASSERT_STACK(stk, "push")

ON_PRINTING(
    printing_stack (stk, "push");
)
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

ON_CANARY_IF(
ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> l_canary_data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)
)
ON_CANARY_ELSE(
ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)
)

    ASSERT_STACK(stk, "pop")
    
ON_PRINTING(
    printing_stack (stk, "pop");
)
    return 1;
}

Elem_t* expansion (Stack_t *stk)
{
ON_CANARY_IF( 
    (stk -> capacity) = (stk -> capacity) * MULTIPLIER;                                                                                                                                     
    stk -> l_canary_data = (Canary_t*) realloc ((stk -> l_canary_data), size_data(stk));     
    void* new_place = nullptr;                                                                                                                                                              
    new_place = (void*)((stk -> l_canary_data) + 1);                                                                                                                                        
    mem_poison (new_place + sizeof(Elem_t)*(stk -> size), (stk -> capacity)-(stk -> size));                                                                        
    ptr_r_canary_data (stk);                                                                                                                                                                
    *(stk -> r_canary_data) = 0xDEADBEEF; 

ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> l_canary_data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)
)
ON_CANARY_ELSE( 
    (stk -> capacity) = (stk -> capacity) * MULTIPLIER;                                                                                      
    void* new_place = realloc ((stk -> data), sizeof(Elem_t)*(stk -> capacity));                                                 
    mem_poison (new_place + sizeof(Elem_t)*(stk -> size), (stk -> capacity)-(stk -> size));                        

ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)
)
    return (Elem_t*) new_place;
}

Elem_t* comprassion (Stack_t *stk)
{
ON_CANARY_IF( 
    (stk -> capacity) = (stk -> capacity) / MULTIPLIER;
    stk -> l_canary_data = (Canary_t*) realloc ((stk -> l_canary_data), size_data(stk));
    void* new_place = nullptr;
    new_place = (void*)((stk -> l_canary_data) + 1);
    ptr_r_canary_data (stk);
    *(stk -> r_canary_data) = 0xDEADBEEF;
ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> l_canary_data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)
    return (Elem_t*) new_place;
)
ON_CANARY_ELSE( 
    (stk -> capacity) = (stk -> capacity) / MULTIPLIER;
ON_HASH(
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func((const char*)(stk -> data), size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
)                             
    return (Elem_t*) realloc ((stk -> data), sizeof(Elem_t)*(stk -> capacity));
)
}

int mem_poison (void* memptr, size_t num)
{
    for (size_t i = 0; i < num; i++)
    {
        *(((Elem_t*) memptr) + i) = POISON_ELEMENT;
    }
    return 1;
}

int StackDtor (Stack_t *stk)
{
ON_CANARY_IF( 
    free(stk -> l_canary_data);        
)
ON_CANARY_ELSE( 
    free(stk -> data);
)
    return 1;
}

int checkStack (Stack_t *stk, char* func)
{
    int error_code = 0;
    
    if (!stk)                    
    {
        error_code += 1<<0;
        return error_code;
    }
    if (!(stk -> data))
    {
        error_code += 1<<1;
        return error_code;
    }
    if ((stk -> size) > (stk -> capacity))                          
    {
        error_code += 1<<2;
    }
    if ((stk -> size) < 0)
    {
        error_code += 1<<3;
    }
    if ((stk -> capacity) <= 0)                                     
    {
        error_code += 1<<4;
    }
ON_CANARY(
    if ((stk -> l_canary)       != 0xDEADBABE)
    {
        error_code += 1<<5;  
    }
    if ((stk -> r_canary)       != 0xDEADBABE)                      
    {
        error_code += 1<<6;   
    }
    if (*(stk -> l_canary_data) != 0xDEADBEEF)                      
    {
        error_code += 1<<7;  
    }
    if (*(stk -> r_canary_data) != 0xDEADBEEF)                      
    {
        error_code += 1<<8;
    }  
)
ON_CANARY_IF(
ON_HASH(
    Hash_t save_hash = (stk -> hash_struct);
    stk -> hash_struct = 0;
    if ((save_hash) != hash_func((const char*)(stk), sizeof(Stack_t))) 
    {
        error_code += 1<<9;
        return error_code;
    }
    else stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
    if ((stk -> hash_data) != hash_func((const char*)(stk -> l_canary_data), size_data(stk))) 
    {
        error_code += 1<<10;
        return error_code;
    }
) 
)
ON_CANARY_ELSE(
ON_HASH(
    Hash_t save_hash = (stk -> hash_struct);
    stk -> hash_struct = 0;
    if ((save_hash) != hash_func((const char*)(stk), sizeof(Stack_t))) 
    {
        error_code += 1<<9;
        return error_code;
    }
    else stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
    if ((stk -> hash_data) != hash_func((const char*)(stk -> data), size_data(stk))) 
    {
        error_code += 1<<10;
        return error_code;
    }
)
)
    if ((!strcmp(func, "pop"))&&((stk -> size) <= 0))  error_code += 1<<11;

    return error_code;
}

int output_error (Stack_t *stk, const char* file, const size_t line, const char* pretty_function, int error_code)
{   
    const char* mass_of_errors[N_ERRORS] = {
        "ADDRESS OF STRUCTURE == NULL",
        "ADDRESS OF ARRAY IN STRUCTURE == NULL",
        "SIZE > CAPACITY",
        "SIZE < 0",
        "CAPACITY <= 0",
        "RIGHT CANARY IN STRUCT IS DEAD. ANAL PENETRATION IN STRUCT",
        "LEFT CANARY IN STRUCT IS DEAD. ANAL PENETRATION IN STRUCT",
        "RIGHT CANARY IN DATA IS DEAD. ANAL PENETRATION IN DATA",
        "LEFT CANARY IN DATA IS DEAD. ANAL PENETRATION IN DATA",
        "HASH BROKE. ANAL PENETRATION IN STRUCT",
        "HASH BROKE. ANAL PENETRATION IN DATA",
        "POP() BUT DATA EMPTY"
    };
    printf("\n<<<<<<<<<<<<<<<YOU HAVE ERROR>>>>>>>>>>>>>>>>>\n");
    int z = error_code;
    int bin_error = 0;
    size_t element = 0;
    int fatal_error = 0;
    printf("\nERROR:\n");
    while (z > 0)
    {
        if (z % 2)
        {
            bin_error += degree(10, element);
            printf("1: %s\n", mass_of_errors[element]);
            if(element != 3) fatal_error = 1;
        }
        else
        {
            printf("0: OK\n");
        }
        z = z / 2;
        element++;
    }
    printf("ERROR CODE: %d\n\n", bin_error);
    if (!fatal_error)
    {
    printf ("Stack[%p] called from %s (string: %d) in function %s\n"
    //      "\t\t%s             \n"
            "{                  \n"
            "\tsize        = %d \n"
            "\tcapacity    = %d \n"
        ON_HASH(
            "\thash_data   = %d \n"
            "\thash_struct = %d \n"
        )
            "\tdata[%p]         \n"
            "\t{                \n", stk, file, line, pretty_function, stk -> size, stk -> capacity, ON_HASH(stk -> hash_data, stk -> hash_struct,) stk -> data);

    for (size_t i = 0; (i < (stk -> capacity)); i++)
    {
        if (i == stk -> size)                               printf("\t >[%d] = %d<\n", i, *((stk -> data) + i));
        else if (*((stk -> data) + i) != POISON_ELEMENT)    printf("\t #[%d] = %d\n", i, *((stk -> data) + i));
        else                                                printf("\t @[%d] = %d(POISON)\n", i, *((stk -> data) + i));
    }
    
    printf ("\t} \n"
            "}   \n");
    }
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
    printf("Size: %d \t Capacity: %d" ON_HASH("\tHASH_DATA: %d \t HASH_STRUCT: %d\n"), (stk -> size), (stk -> capacity) ON_HASH(, (stk -> hash_data), (stk -> hash_struct)));
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

ON_CANARY(
Canary_t* ptr_r_canary_data (Stack* stk)                                                                                                    
{                                                                                                                                           
    Canary_t* ptr = (Canary_t*)((char*)((stk -> data) + (stk -> capacity))+((stk -> capacity)*sizeof(Elem_t)) % sizeof(Canary_t));          
    stk -> r_canary_data = ptr;                                                                                                             
    return ptr;                                                                                                                             
}   
)

ON_HASH(
Hash_t hash_func(const char * str, size_t size)
{
    return MYHashFAQ6(str, size);
}

Hash_t MYHashFAQ6(const char * str, size_t size)
{
    unsigned int hash = 0;
    const char* z = str;
    for (; str < (z + size); str++)
    {
        hash += (unsigned char)(*str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}
)

size_t size_data (Stack* stk)
{
ON_CANARY_IF(
    return sizeof(Canary_t)*2 + sizeof(Elem_t)*(stk -> capacity) + ((stk -> capacity)*sizeof(Elem_t)) % sizeof(Canary_t);
)
ON_CANARY_ELSE(
    return sizeof(Elem_t)*(stk -> capacity);
)
}
