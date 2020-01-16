/* Linked List Data is a union which could be character, integer, float, or double */
typedef union {
        char char_value;
        int int_value;
        float float_value;
        double double_value;
} LinkedListData;

/* Linked List Node is a structure */
struct linked_list_node {
    LinkedListData data;
    struct linked_list_node* next;
};
typedef struct linked_list_node LinkedListNode;
typedef LinkedListNode* LinkedListNodePtr;

void insertLinkedList (LinkedListNodePtr* lastNodePtr, LinkedListData newData);
void printLinkedList (LinkedListNodePtr curNode, char type);
void deleteLinkedList (LinkedListNodePtr* curNodePtr);
int isLinkedListEmpty (LinkedListNodePtr firstNode);