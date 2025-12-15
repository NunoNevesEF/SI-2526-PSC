#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>

struct table
{
    int nCols;
    int nRows;
    char **columns;
    char ***rows;
};

struct table *table_load_csv(const char *filename);
void table_save_csv(const struct table *table, const char *filename);
struct table *table_filter(const struct table *table, bool (*predicate)(const void *row, const void *context), const void *context);

#endif
