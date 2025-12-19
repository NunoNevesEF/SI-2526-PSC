#include <stdio.h>
#include "table.h"

void handle_remove_first(char *args[])
{
    if (current_table == NULL || current_table->nRows == 0)
    {
        printf("No rows to remove\n");
        return;
    }

    for (int r = 1; r < current_table->nRows; r++)
    {
        current_table->rows[r - 1] = current_table->rows[r];
    }

    current_table->nRows--;

    printf("First row removed\n");
}