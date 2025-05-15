/* llist.c
 * Generic Linked List implementation
 */

#include <stdlib.h>
#include "llist.h"

// Create a new linked list
LinkedList *llist_create()
{
    LinkedList *llist = (LinkedList *)malloc(sizeof(LinkedList));

    llist->size = 0;
    llist->head = NULL;
    llist->tail = NULL;

    return llist;
}

// Push a new node to the end of the linked list
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

// Remove a node from the linked list
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

// Pop the first node from the linked list
Node *llist_pop(LinkedList *llist)
{
    Node *node = llist->head;

    if (node != NULL)
    {
        llist->head = node->next;
        llist->size--;

        if (llist->size == 0)
            llist->tail = NULL;
    }

    return node;
}
