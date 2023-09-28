#include "head.h"

int main(void)
{
    Stack_t myStack = {};

    stackCtor (&myStack);

    int x = 0;

    push (&myStack, 40);
    push (&myStack, 30);
    push (&myStack, 1);
    push (&myStack, 9);
    push (&myStack, -11);
    push (&myStack, 4);
    push (&myStack, -11);
    push (&myStack, 4);    
    push (&myStack, -11);
    push (&myStack, 4);
    push (&myStack, -11);
    push (&myStack, 4);

    pop  (&myStack, &x);
    pop  (&myStack, &x);
    pop  (&myStack, &x);
    pop  (&myStack, &x);
    pop  (&myStack, &x);
    pop  (&myStack, &x);
    pop  (&myStack, &x);
    pop  (&myStack, &x);
    pop  (&myStack, &x);
    pop  (&myStack, &x);

#if (DEBUG_PRINTING != 0)
    printing_stack (&myStack, "main");
#endif

    StackDtor (&myStack);
    return 0;
}