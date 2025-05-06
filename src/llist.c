/* llist.c
 * Generic Linked List implementation
 */

#include <stdlib.h>

typedef struct
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

LinkedList *llist_create()
{
    LinkedList *llist = (LinkedList *)malloc(sizeof(LinkedList));

    llist->size = 0;
    llist->head = NULL;
    llist->tail = NULL;

    return llist;
}

void llist_push(LinkedList *llist, void *data)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->data = data;
    node->next = NULL;

    if (llist->size > 0)
    {
        llist->tail->next = node;
        llist->tail = node;
    }
    else
    {
        llist->head = node;
        llist->tail = node;
    }

    llist->size++;
}

int llist_remove(LinkedList *llist, void *data)
{
    Node *current = llist->head;
    Node *previous = NULL;

    while (current != NULL)
    {
        if (current->data == data)
        {
            if (previous == NULL)
                llist->head = current->next;
            else
                previous->next = current->next;

            if (current == llist->tail)
                llist->tail = previous;

            free(current);
            llist->size--;

            return 0;
        }

        previous = current;
        current = current->next;
    }

    return -1;
}