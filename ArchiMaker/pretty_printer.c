#include "archimaker.h"

int enqueue(struct queue *q, struct tree *root) {

    // if q == NULL then create queue and enqueue node
    if (!q) {

        q = calloc(1, sizeof(struct queue));
        if (!q) {
            fprintf(stderr, "Encountered an error while allocating memory for object: struct queue. Abort.\n");
            return 1;
        }

    }

    // Creating the node (linked list) containing the root
    struct node *node = calloc(1, sizeof(struct node));
    if (!node) {
        fprintf(stderr, "Encountered an error while allocating memory for object: struct node. Abort.\n");
        return 1;
    }
    node->data = root;

    // if queue is empty
    if (!q->head)
        q->head = node;
    else {
        struct node *curr = q->head;
        while (curr->next)
            curr = curr->next;
        curr->next = node;
    }

    return 0;

}

struct tree *dequeue(struct queue *q) {

    // if empty queue
    if (!q->head)
        return NULL;

    struct node *node = q->head;
    q->head = q->head->next;
    node->next = NULL;

    struct tree *out = node->data;
    free(node);
    return out;

}
