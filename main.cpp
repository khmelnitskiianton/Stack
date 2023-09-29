#include "head.h"

int main(void)
{
    Stack_t myStack = {};
    myStack.file_write = file_write();
    stackCtor (&myStack);

    int x = 0;
    push (&myStack, 40);
    push (&myStack, 30);
    push (&myStack, 1);
    push (&myStack, 9);
    push (&myStack, -11);
    push (&myStack, 4);
    myStack.capacity = 100000000000000;
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

ON_PRINTING(
    printing_stack (&myStack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
)

    StackDtor (&myStack);
    file_close(myStack.file_write);
    return 0;
}