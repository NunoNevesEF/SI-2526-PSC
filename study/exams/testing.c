#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
1024   512   256   128   64   32  16  8   4   2   1
*/

typedef struct list_node
{
    struct list_node *link;
    void *data;
} Lnode;

Lnode *list_copy_deep(Lnode *list, void *(*copy)(void *))
{
    Lnode *new_head = malloc(sizeof(Lnode));
    new_head->data = copy(list->data);
    new_head->link = NULL;

    Lnode *current_new = new_head;
    Lnode *current_old = list->link;

    while (current_old != NULL)
    {
        Lnode *new_node = malloc(sizeof(Lnode));
        new_node->data = copy(current_old->data);
        new_node->link = NULL;

        current_new->link = new_node;
        current_new = new_node;
        current_old = current_old->link;
    }

    return new_head;
}

void list_free_deep(Lnode *list, void (*pfree)(void *))
{
    Lnode *current = list;
    while (current != NULL)
    {
        Lnode next = current->link;
        pfree(current->data);
        free(current);
        current = next;
    }
}

int main()
{

    // bool flag = false;
    // int result = round_power2(13, &flag);
    // printf("Result = %i \n", result);  // int

    // char palindrome[] = "roma e amor";
    // bool result = is_palindrome(palindrome);
    // printf("Result = %s \n", result ? "true" : "false");

    return 0;
}