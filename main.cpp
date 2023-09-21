#include "head.h"

int main(void)
{
    Stack_t myStack = {};

    stackCtor (&myStack);

    push (&myStack, 40);
    push (&myStack, 30);
    push (&myStack, 1);
    push (&myStack, 0);
    push (&myStack, -11);
    push (&myStack, 2);
    push (&myStack, 4);
    pop  (&myStack);
    pop  (&myStack);
    pop  (&myStack);
    pop  (&myStack);
    
    /*
    for (size_t i = 0; i < myStack.capacity; i++)
    {
        printf("stack[%p] = %d\n", myStack.data + i, *(myStack.data + i));
    }
    printf("Size: %d \t Capacity: %d", myStack.size, myStack.capacity);
    */

    int StackDtor (Stack_t *stk);
    return 0;
}