struct queue_node {
    int data;
    struct queue_node* next;   
};
typedef struct queue_node QueueNode;
typedef QueueNode* QueueNodePtr;
typedef struct {
    QueueNodePtr head;
    QueueNodePtr tail;
} Queue;
typedef Queue* QueuePtr;

int deQueue (QueuePtr queuePtr);
void enQueue (QueuePtr queuePtr, int data);
int isQueueEmpty (const Queue q); 