#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <csv.h>

/*
struct table *table_load_csv_legacy(const char *filename)
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
*/

static struct table *t = NULL;

/**
 * cb1
 */
void on_field(void *s, size_t len, void *data)
{
    char *field = strndup(s, len);

    if (!t->state.cols_done)
    {
        t->columns = realloc(
            t->columns,
            (t->nCols + 1) * sizeof(char *));
        t->columns[t->nCols++] = field;
    }
    else
    {
        t->state.current_row[t->state.current_col++] = field;
    }
}

/**
 * cb2
 */
void on_row(int c, void *data)
{
    if (!t->state.cols_done)
    {
        t->state.cols_done = true;
        t->state.current_row = malloc(t->nCols * sizeof(char *));
        t->state.current_col = 0;
        return;
    }

    t->rows = realloc(t->rows, (t->nRows + 1) * sizeof(char **));
    t->rows[t->nRows++] = t->state.current_row;

    t->state.current_row = malloc(t->nCols * sizeof(char *));
    t->state.current_col = 0;
}

struct table *table_load_csv(const char *filename)
{
    struct csv_parser p;
    t = malloc(sizeof(struct table));

    // Data
    t->nCols = 0;
    t->nRows = 0;
    t->columns = NULL;
    t->rows = NULL;

    // State
    t->state.cols_done = false;
    t->state.current_row = NULL;
    t->state.current_col = 0;

    csv_init(&p, 0);

    FILE *fp = fopen(filename, "r");
    char buf[1024];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
    {
        if (csv_parse(&p, buf, n, on_field, on_row, NULL) != n)
        {
            fprintf(stderr, "CSV error: %s\n",
                    csv_strerror(csv_error(&p)));
            break;
        }
    }

    csv_fini(&p, NULL, NULL, NULL);
    csv_free(&p);
    fclose(fp);

    return t;
}

void cb1(void *s, size_t i, void *outfile)
{
    fwrite(s, 1, i, (FILE *)outfile);
    fputc(',', (FILE *)outfile);
}

void cb2(int c, void *outfile)
{
    FILE *f = (FILE *)outfile;
    fseek(f, -1, SEEK_CUR); // Corrects the last comma
    fputc('\n', f);
}

void table_save_csv(const struct table *table, const char *filename)
{
    char fileLocation[256] = "generatedFiles/";
    strcat(fileLocation, filename);
    FILE *fptr = fopen(fileLocation, "w");

    for (int c = 0; c < table->nCols; c++)
    {
        cb1(table->columns[c], strlen(table->columns[c]), fptr);
    }

    cb2(0, fptr);

    for (int r = 0; r < table->nRows; r++)
    {
        for (int c = 0; c < table->nCols; c++)
        {
            cb1(table->rows[r][c], strlen(table->rows[r][c]), fptr);
        }
        cb2(0, fptr);
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
