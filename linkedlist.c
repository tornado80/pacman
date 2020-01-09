#include <stdlib.h>

struct LinkedListNode;
union LinkeDListData;
typedef LinkedListNode* LinkedListNodePtr;

/* Linked List Data is a union which could be character, integer, float, or double */
typedef union {
        char char_value;
        int int_value;
        float float_value;
        double double_value;
} LinkeDListData;

/* Linked List Node is a structure */
typedef struct {
    LinkeDListData data;
    LinkedListNodePtr next;
} LinkedListNode;

void insertLinkedList ();