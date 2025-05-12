#ifndef LLIST_H
#define LLIST_H

typedef struct Node
{
    void *data;        // Pointer to the data
    struct Node *next; // Pointer to the next node
} Node;

typedef struct
{
    int size;   // Size of the linked list
    Node *head; // Pointer to the head node
    Node *tail; // Pointer to the tail node
} LinkedList;

/**
 * @brief Creates a new linked list.
 */
LinkedList *llist_create();

/**
 * @brief Pops the first node from the linked list and returns it.
 *
 * @param llist Pointer to the linked list.
 *
 * @return Pointer to the popped node.
 */
Node *llist_pop(LinkedList *llist);

/**
 * @brief Pushes a new node with the given data to the end of the linked list.
 *
 * @param llist Pointer to the linked list.
 * @param data Pointer to the data to be added.
 */
void llist_push(LinkedList *llist, void *data);

/**
 * @brief Removes the first node with the given data from the linked list.
 *
 * @param llist Pointer to the linked list.
 * @param data Pointer to the data to be removed.
 *
 * @return 0 on success, -1 if the data was not found.
 */
int llist_remove(LinkedList *llist, void *data);

#endif
