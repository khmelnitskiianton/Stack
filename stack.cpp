#include "head.h"

int stackCtor (Stack_t *stk)
{
    stk -> file_write = file_write();
    stk -> size = 0; 
    stk -> capacity = 1;
    stk -> ret_value = 1;

ON_CANARY_IF(                                                
    stk -> l_canary = 0xDEADBABE;                                                                                                                   
    stk -> l_canary_data = (Canary_t*) calloc (1, size_data(stk));     
    stk -> data = (Elem_t*) ((stk -> l_canary_data) + 1);                                                                                           
    ptr_r_canary_data (stk);                                                                                                                        
    *(stk -> l_canary_data) = 0xDEADBEEF;                                                                                                           
    *(stk -> r_canary_data) = 0xDEADBEEF;                                                                                                           
    stk -> r_canary = 0xDEADBABE; 
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)                                      
)
ON_CANARY_ELSE( 
    stk -> data = (Elem_t*) calloc (1, size_data(stk));     
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)
)
    return 1;
}

int push (Stack_t *stk, Elem_t value)
{ 
    ASSERT_STACK(stk, __PRETTY_FUNCTION__)
    if ((stk -> size) == (stk -> capacity))
    {
        (stk -> data) = expansion (stk);
    }

    *((stk -> data) + (stk -> size)) = value;
    (stk -> size)++; 

ON_CANARY_IF(
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)
)
ON_CANARY_ELSE(
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)
)
    ASSERT_STACK(stk, __PRETTY_FUNCTION__)

ON_PRINTING(
    printing_stack (stk, __FILE__, __LINE__, __PRETTY_FUNCTION__);
)
    return 1;
}

int pop (Stack_t *stk, Elem_t* ret_value)
{
    if (!stk) 
    {
        ASSERT_STACK(stk, __PRETTY_FUNCTION__)
    }
    else if (ret_value == NULL) 
    {
        stk -> ret_value = 0;
        ASSERT_STACK(stk, __PRETTY_FUNCTION__)
    }
    else 
    {
        stk -> ret_value = 1;
        ASSERT_STACK(stk, __PRETTY_FUNCTION__)
    }

    Elem_t save_value = POISON_ELEMENT;
    (stk -> size)--;
    save_value = *((stk -> data) + (stk -> size));  
    if (((stk -> size) > 0) && ((stk -> size)*MULTIPLIER < (stk -> capacity)) && ((stk -> capacity) > MIN_LEN))
    {
        (stk -> data) = comprassion (stk);
    }
    *((stk -> data) + (stk -> size)) = POISON_ELEMENT;
    *ret_value = save_value;

ON_CANARY_IF(
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)
)
ON_CANARY_ELSE(
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)
)
    ASSERT_STACK(stk, __PRETTY_FUNCTION__)

ON_PRINTING(
    printing_stack (stk, __FILE__, __LINE__, __PRETTY_FUNCTION__);
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
    mem_poison ((char*) new_place + sizeof(Elem_t)*(stk -> size), (stk -> capacity)-(stk -> size));                                                                        
    ptr_r_canary_data (stk);                                                                                                                                                                
    *(stk -> r_canary_data) = 0xDEADBEEF; 
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)
)
ON_CANARY_ELSE( 
    (stk -> capacity) = (stk -> capacity) * MULTIPLIER;                                                                                      
    void* new_place = realloc ((stk -> data), sizeof(Elem_t)*(stk -> capacity));                                                 
    mem_poison (new_place + sizeof(Elem_t)*(stk -> size), (stk -> capacity)-(stk -> size));                        
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
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
    rewrite_hash(stk, (const char*)(stk -> l_canary_data));
)
)
ON_CANARY_ELSE( 
    (stk -> capacity) = (stk -> capacity) / MULTIPLIER;
    void* new_place = realloc ((stk -> data), sizeof(Elem_t)*(stk -> capacity));
ON_HASH(
    rewrite_hash(stk, (const char*)(stk -> data));
)                             
)
    return (Elem_t*) new_place;
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
    ASSERT_STACK(stk, __PRETTY_FUNCTION__)
ON_CANARY_IF( 
    free(stk -> l_canary_data);        
    stk -> l_canary_data = NULL;
    stk -> r_canary_data = NULL;
)
ON_CANARY_ELSE( 
    free(stk -> data);
)
    file_close(stk -> file_write);
    stk -> data = NULL;
    stk = NULL;
    return 1;
}

int checkStack (Stack_t *stk, const char* func)
{
    int error_code = 0;
    
    if (!stk)                    
    {
        error_code += 1<<0;
        return error_code;
    }
    if (!(stk -> file_write))
    {
        error_code += 1<<1;
        return error_code;
    }
    if (!(stk -> data))
    {
        error_code += 1<<2;
        return error_code;
    }
    if ((strcmp(func, "int pop(Stack_t*, Elem_t*)") == 0)&&(!(stk -> ret_value)))
    {
        error_code += 1<<3;
        return error_code;
    }

    if ((stk -> size) > (stk -> capacity))  error_code += 1<<4;
    if ((stk -> size) < 0)                  error_code += 1<<5;
    if ((stk -> capacity) <= 0)             error_code += 1<<6;


    if ((strcmp(func, "int pop(Stack_t*, Elem_t*)") == 0)&&((stk -> size) == 0)) error_code += 1<<7;

ON_CANARY(
    if ((stk -> l_canary)       != 0xDEADBABE) error_code += 1<<8; 
    if ((stk -> r_canary)       != 0xDEADBABE) error_code += 1<<9;   
    if (*(stk -> l_canary_data) != 0xDEADBEEF) error_code += 1<<10;  
    if (*(stk -> r_canary_data) != 0xDEADBEEF) error_code += 1<<11;
)
ON_CANARY_IF(
ON_HASH(
    Hash_t save_hash = (stk -> hash_struct);
    stk -> hash_struct = 0;
    if ((save_hash) != hash_func((const char*)(stk), sizeof(Stack_t))) 
    {
        error_code += 1<<12;
        return error_code;
    }
    else stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
    if ((stk -> hash_data) != hash_func((const char*)(stk -> l_canary_data), size_data(stk))) error_code += 1<<13;
) 
)
ON_CANARY_ELSE(
ON_HASH(
    Hash_t save_hash = (stk -> hash_struct);
    stk -> hash_struct = 0;
    if ((save_hash) != hash_func((const char*)(stk), sizeof(Stack_t))) 
    {
        error_code += 1<<12;
        return error_code;
    }
    else stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
    if ((stk -> hash_data) != hash_func((const char*)(stk -> data), size_data(stk))) error_code += 1<<13;
)
)
    return error_code;
}

int output_error (Stack_t *stk, const char* file, const size_t line, const char* pretty_function, int error_code)
{   
    const char* mass_of_errors[N_ERRORS] = {
        "ADDRESS OF STRUCTURE == NULL. OUTPUT IN TERMINAL",
        "ADDRESS OF FILE WRITE == NULL. OUTPUT IN TERMINAL",
        "ADDRESS OF ARRAY IN STRUCTURE == NULL",
        "ADDRESS OF RETURNING ARGUMENT IN POP == NULL",
        "SIZE > CAPACITY",
        "SIZE < 0",
        "CAPACITY <= 0",
        "POP() BUT DATA EMPTY",
        "LEFT CANARY IN STRUCT IS DEAD. ANAL PENETRATION IN STRUCT",
        "RIGHT CANARY IN STRUCT IS DEAD. ANAL PENETRATION IN STRUCT",
        "LEFT CANARY IN DATA IS DEAD. ANAL PENETRATION IN DATA",
        "RIGHT CANARY IN DATA IS DEAD. ANAL PENETRATION IN DATA",
        "HASH BROKE. ANAL PENETRATION IN STRUCT",
        "HASH BROKE. ANAL PENETRATION IN DATA"
    };
    int z = error_code;
    int bin_error = 0;
    size_t element = 0;
    int fatal_error = 0;
    if (z == 1)
    {
        printf("\n<<<<<<<<<<<<<<<YOU HAVE ERROR>>>>>>>>>>>>>>>>>\n"
            "Stack[%p] called from %s (string: %d) in function %s\n"
            "1: [%s]\n", stk, file, line, pretty_function,mass_of_errors[0]);
        abort();
    }
    if (z == 2)
    {
        printf("\n<<<<<<<<<<<<<<<YOU HAVE ERROR>>>>>>>>>>>>>>>>>\n"
            "Stack[%p] called from %s (string: %d) in function %s\n"
            "0: [OK]\n"
            "1: [%s]\n", stk, file, line, pretty_function,mass_of_errors[1]);
        abort();
    }
    fprintf(stk -> file_write, "\n<<<<<<<<<<<<<<<YOU HAVE ERROR>>>>>>>>>>>>>>>>>\n");
    fprintf(stk -> file_write, "\nERROR:\n");

    while (z > 0)
    {
        if (z % 2)
        {
            bin_error += degree(10, element);
            fprintf(stk -> file_write, "1: [%s]\n", mass_of_errors[element]);
            if(element != 13) fatal_error = 1;
        }
        else
        {
            fprintf(stk -> file_write, "0: [OK]\n");
        }
        z = z / 2;
        element++;
    }
    fprintf(stk -> file_write,"ERROR CODE: %d\n\n", bin_error);
    
    if (!fatal_error)
    {
    fprintf (stk -> file_write,"Stack[%p] called from %s (string: %d) in function %s\n"
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

    for (ssize_t i = 0; (i < (stk -> capacity)); i++)
    {
        if (i == stk -> size)                               fprintf(stk -> file_write,"\t >[%d] = %d<\n", i, *((stk -> data) + i));
        else if (*((stk -> data) + i) != POISON_ELEMENT)    fprintf(stk -> file_write,"\t #[%d] = %d\n", i, *((stk -> data) + i));
        else                                                fprintf(stk -> file_write,"\t @[%d] = %d(POISON)\n", i, *((stk -> data) + i));
    }
    
    fprintf (stk -> file_write,"\t} \n"
            "}   \n");
    }
    abort();
    return 1;
}

int printing_stack (Stack* stk, const char* file, const size_t line, const char* pretty_function)
{
    fprintf (stk -> file_write,"\nStack[%p] called from %s (string: %d) in function %s\n"
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

    for (ssize_t i = 0; (i < (stk -> capacity)); i++)
    {
        if (i == (stk -> size))                             fprintf(stk -> file_write,"\t >[%d] = %d<\n", i, *((stk -> data) + i));
        else if (*((stk -> data) + i) != POISON_ELEMENT)    fprintf(stk -> file_write,"\t #[%d] = %d\n", i, *((stk -> data) + i));
        else                                                fprintf(stk -> file_write,"\t @[%d] = %d(POISON)\n", i, *((stk -> data) + i));
    }
    
    fprintf (stk -> file_write,"\t} \n"
            "}   \n");
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

Canary_t* ptr_r_canary_data (Stack* stk)                                                                                                    
{                                                                                                                                           
    Canary_t* ptr = (Canary_t*)((char*)((stk -> data) + (stk -> capacity))+((stk -> capacity)*sizeof(Elem_t)) % sizeof(Canary_t));          
    stk -> r_canary_data = ptr;                                                                                                             
    return ptr;                                                                                                                             
}   


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

size_t size_data (Stack* stk)
{
ON_CANARY_IF(
    return sizeof(Canary_t)*2 + sizeof(Elem_t)*(stk -> capacity) + ((stk -> capacity)*sizeof(Elem_t)) % sizeof(Canary_t);
)
ON_CANARY_ELSE(
    return sizeof(Elem_t)*(stk -> capacity);
)
}

FILE* file_write (void)
{
    FILE *file_write = nullptr;
    if ((file_write = fopen (FILE_WRITE, "w")) == nullptr) 
    {
        assert(0);
    }
    return file_write;
}

int file_close (FILE *file_text)
{
	if (!file_text)
	{
		assert(0);
	}
	if (fclose (file_text) != 0)
	{
		assert(0);
	}
	return 1;
}

int rewrite_hash (Stack* stk, const char* ptr)
{
    stk -> hash_struct = 0;
    stk -> hash_data = hash_func(ptr, size_data(stk));
    stk -> hash_struct = hash_func((const char*)(stk), sizeof(Stack_t));
    return 1;
}