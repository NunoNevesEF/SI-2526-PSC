#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct table *table_load_csv(const char *filename)
{
    FILE *stream = fopen(filename, "r");
    struct table *t = malloc(sizeof(struct table));
    t->nCols = 0;
    t->nRows = 0;
    t->columns = NULL;
    t->rows = NULL;

    char line[1024];
    bool columnsProcessed = false;
    while (fgets(line, sizeof(line), stream))
    {
        if (!columnsProcessed)
        {
            // Process columns for table->columns e preencher nCols
            char *tok = strtok(line, ",");
            while (tok)
            {
                t->columns = realloc(t->columns, (t->nCols + 1) * sizeof(char *));
                t->columns[t->nCols] = strdup(tok);
                t->nCols++;
                tok = strtok(NULL, ",");
            }
            columnsProcessed = true;
        }
        else
        {
            // Process lines for table->lines e preencher nLines
            t->rows = realloc(t->rows, (t->nRows + 1) * sizeof(char **));
            t->rows[t->nRows] = malloc(t->nCols * sizeof(char *));

            char *tok = strtok(line, ",");
            for (int i = 0; i < t->nCols; i++)
            {
                t->rows[t->nRows][i] = strdup(tok);
                tok = strtok(NULL, ",");
            }

            t->nRows++;
        }
    }

    fclose(stream);

    return t;
}

void table_save_csv(const struct table *table, const char *filename)
{
    FILE *fptr;
    char fileLocation[256] = "generatedFiles/";
    strcat(fileLocation, filename);

    fptr = fopen(fileLocation, "w");

    for (int i = 0; i < table->nCols; i++)
    {
        fprintf(fptr, "%s", table->columns[i]);
        if (i < table->nCols - 1)
            fprintf(fptr, ",");
    }

    for (int j = 0; j < table->nRows; j++)
    {
        for (int k = 0; k < table->nCols; k++)
        {
            fprintf(fptr, "%s", table->rows[j][k]);
            if (k < table->nCols - 1)
                fprintf(fptr, ",");
        }
    }

    fclose(fptr);
}

struct table *table_filter(const struct table *table, bool (*predicate)(const void *row, const void *context), const void *context)
{
    struct table *newTable = malloc(sizeof(struct table));
    newTable->nCols = table->nCols;
    newTable->nRows = 0;
    newTable->columns = malloc(table->nCols * sizeof(char *));
    newTable->rows = NULL;

    for (int i = 0; i < table->nCols; i++)
    {
        newTable->columns[i] = strdup(table->columns[i]);
    }

    for (int r = 0; r < table->nRows; r++)
    {
        const char **row = (const char **)table->rows[r];

        if (predicate(row, context))
        {
            newTable->rows = realloc(newTable->rows, (newTable->nRows + 1) * sizeof(char **));
            newTable->rows[newTable->nRows] = malloc(newTable->nCols * sizeof(char *));

            for (int c = 0; c < newTable->nCols; c++)
            {
                newTable->rows[newTable->nRows][c] = strdup(table->rows[r][c]);
            }
            newTable->nRows++;
        }
    }

    return newTable;
}

bool price_lower_than_euro(const void *row, const void *context)
{
    char **linha = (char **)row;
    int limit = *(int *)context;

    int price = atoi(linha[4]);

    return price < limit;
}
