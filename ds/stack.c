#include <stdlib.h>
#include "./stack.h"

int isStackEmpty(StackNodePtr stackTopNode) {
    return stackTopNode == NULL;
}

void pushStack (StackNodePtr* stackTopNodePtr, int value) {
    StackNodePtr newNode = malloc(sizeof(StackNode));
    newNode->data = value;
    newNode->next = *stackTopNodePtr;
    *stackTopNodePtr = newNode;
}

int popStack (StackNodePtr* stackTopNodePtr) {
    if (isStackEmpty(*stackTopNodePtr))
        return -1;
    StackNodePtr temp = *stackTopNodePtr;
    *stackTopNodePtr = temp->next;
    int value = temp->data;
    free(temp);
    return value;
}