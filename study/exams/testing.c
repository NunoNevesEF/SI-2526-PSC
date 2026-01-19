#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
1024   512   256   128   64   32  16  8   4   2   1
*/

/*

b. [2,5] Escreva em linguagem C, um trecho de programa de teste da função get_val_ptr, que defina uma
estrutura de dados estática e invoque a função de modo que retorne um ponteiro diferente de NULL.

*/

struct data
{
    short flags : 6;
    short length : 10;
    short *vals;
};
struct info
{
    double ref;
    struct data **data;
    int valid;
};
short *get_val_ptr(struct info items[],
                   size_t item_idx, size_t data_idx, size_t val_idx, short mask)
{
    return items[item_idx].data[data_idx]‐> flags & mask
               ? &items[item_idx].data[data_idx]‐> vals[val_idx]
               : NULL;
}

int main()
{
    struct data d0;
    short vals_array[3] = {10, 20, 30};

    d0.flags = 1;
    d0.length = 3;
    d0.vals = vals_array;

    struct data *data_ptrs[1];
    data_ptrs[0] = &d0;

    struct info items[1];
    items[0].ref = 3.14;
    items[0].data = data_ptrs;
    items[0].valid = 1;

    short mask = 1;
    short *result = get_val_ptr(items, 0, 0, 1, mask);
}