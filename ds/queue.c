#include <stdlib.h>
#include <stdio.h>
#include "./queue.h"

int isQueueEmpty (const Queue q) {
    if (q.head == NULL && q.tail == NULL)
        return 1;
    else
        return 0;
}

void enQueue (QueuePtr queuePtr, int data) {
    QueueNodePtr newNodePtr = malloc(sizeof(QueueNode));
    newNodePtr->data = data;
    newNodePtr->next = NULL;
    if (isQueueEmpty(*queuePtr)) {
        queuePtr->head = queuePtr->tail = newNodePtr;
    } else {
        queuePtr->tail->next = newNodePtr;
        queuePtr->tail = newNodePtr;  
    }
}

int deQueue (QueuePtr queuePtr) {
    if (!isQueueEmpty(*queuePtr)) {
        if (queuePtr->head == queuePtr->tail) {
            int value;
            value = queuePtr->head->data;
            free(queuePtr->tail);
            queuePtr->tail = queuePtr->head = NULL;
            return value;
        } else {
            int value;
            QueueNodePtr temp = queuePtr->head;
            queuePtr->head = temp->next;
            value = temp->data;
            free(temp);
            return value;
        }
    } else {
        printf("EMPTY");
    }
}

