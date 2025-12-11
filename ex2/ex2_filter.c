#include "table.h"
#include <stdio.h>
#include <stdbool.h>

bool price_lower_than_euro(const void *row, const void *context);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Wrong number of arguments, please try again...\n");
        return 1;
    }

    char *inFile = argv[1];
    char *outFile = argv[2];

    int limit = 1;
    struct table *t = table_load_csv(inFile);
    struct table *filtered = table_filter(t, price_lower_than_euro, &limit);
    table_save_csv(filtered, outFile);

    return 0;
}
