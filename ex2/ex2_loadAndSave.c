#include "table.h"
#include <stdio.h>



// ./ex2_loadAndSave frutas.csv frutas_copy_output.csv
// ./csv_compare frutas.csv generatedFiles/frutas_copy_output.csv

// ./ex2_loadAndSave gutenberg_metadata.csv gutenberg_copy_output.csv
// ./csv_compare gutenberg_metadata.csv generatedFiles/gutenberg_copy_output.csv


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Wrong number of arguments, please try again...\n");
        return 1;
    }

    char *inFile = argv[1];
    char *outFile = argv[2];

    struct table *t = table_load_csv(inFile);
    table_save_csv(t, outFile);

    return 0;
}
