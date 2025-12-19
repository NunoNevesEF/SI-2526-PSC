#include "table.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// ./ex2_filter frutas.csv frutas_filter_output.csv

// ./ex2_filter gutenberg_metadata.csv gutenberg_filter_output.csv


bool price_lower_than_euro(const void *row, const void *context);

bool number_lower_than_given(const void *row, const void *context);

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

    struct table *filtered;
    if (strcmp(inFile, "gutenberg_metadata.csv") == 0)
    {
        limit = 2000;
        filtered = table_filter(t, number_lower_than_given, &limit);
    }
    else if (strcmp(inFile, "frutas.csv") == 0)
    {
        filtered = table_filter(t, price_lower_than_euro, &limit);
    } else {
        filtered = table_filter(t, price_lower_than_euro, &limit);
    }

    //struct table *filtered = table_filter(t, price_lower_than_euro, &limit);
    table_save_csv(filtered, outFile);

    return 0;
}
