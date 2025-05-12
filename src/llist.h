#ifndef LLIST_H
#define LLIST_H

typedef struct Node
{
    void *data;
    struct Node *next;
} Node;

typedef struct
{
    int size;
    Node *head;
    Node *tail;
} LinkedList;

LinkedList *llist_create();
Node *llist_pop(LinkedList *llist);
void llist_push(LinkedList *llist, void *data);
int llist_remove(LinkedList *llist, void *data);

#endif
