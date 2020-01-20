#include <stdlib.h>
#include <stdio.h>
#include "./linkedlist.h"

/* Return 1 if linked list is empty otherwise 0 */
int isLinkedListEmpty (LinkedListNodePtr firstNode) {
    return firstNode == NULL;
}

/* 
Insert a node at the end of the list 
*/
void insertLinkedList (LinkedListNodePtr* firstNodePtr, LinkedListData newData) {
    // Using a pointer to first node pointer is just for the case when inserting to an empty list
    LinkedListNodePtr newNode = malloc(sizeof(LinkedListNode)), curNode = *firstNodePtr;
    newNode->data = newData;
    newNode->next = NULL;
    if (isLinkedListEmpty(*firstNodePtr))
        *firstNodePtr = newNode;
    else {
        while (curNode->next != NULL)
            curNode = curNode->next;
        curNode->next = newNode;
    }
}

/* 
Prints all data of a linked list nodes in order after the given node
base on the type of values in character format: 'c' for char, 'i' for integer, 'f' for float, 'd' for double
*/
void printLinkedList (LinkedListNodePtr curNode, char type) {
    while (curNode != NULL) {
        switch (type) {
        case 'c':
            printf("%c -> ", curNode->data.char_value);
            break;
        case 'i':
            printf("%d -> ", curNode->data.int_value);
            break;
        case 'd':
            printf("%lf -> ", curNode->data.double_value);
            break;
        case 'f':
            printf("%f -> ", curNode->data.float_value);
            break;                                
        default:
            printf("ERROR : Given type is unknown!\n");
        }
        curNode = curNode->next;
    }
    printf("NULL\n");
}

/* 
Delete all nodes of a linked list after the given node (including the given node)
So one has to set the previous node next pointer to NULL (this function doesn't do that)
*/
void deleteLinkedList (LinkedListNodePtr* curNodePtr) {
    LinkedListNodePtr curNode = *curNodePtr, nextNode;
    *curNodePtr = NULL;
    while (curNode != NULL) {
        nextNode = curNode->next;
        free(curNode);
        curNode = nextNode;
    }
}