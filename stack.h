struct stack_node {
    int data;
    struct stack_node * next;
};
typedef struct stack_node StackNode;
typedef StackNode* StackNodePtr;

int popStack (StackNodePtr* stackTopNodePtr);
void pushStack (StackNodePtr* stackTopNodePtr, int value);
int isStackEmpty(StackNodePtr stackTopNode);

 