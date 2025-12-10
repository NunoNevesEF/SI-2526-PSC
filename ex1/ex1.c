#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>

/*a. Defina a struct table para suporte em memória dos dados de uma tabela. A tabela tem duas dimensões,
as colunas são identificadas por letras e as linhas são identificadas por números. O número de colunas é
fixado na criação da tabela, num máximo de 26 colunas. O número de linhas é dinâmico, pode aumentar ou
diminuir ao longo do processamento. Na concretização da struct table deve-se minimizar a memória
alocada e não utilizada.
struct table { …campos a definir… };*/

struct table
{
    int nCols;
    int nRows;
    char **columns;
    char ***rows;
};

/*b. Programe a função table_load_csv que cria uma tabela (uma instância de struct table) preenchida com
os dados lidos de um ficheiro em formato CSV. Considere que o ficheiro está sempre bem formado.
struct table *table_load_csv(const char *filename);*/

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
            // Process lines for table->lines e preencher nLines no final
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

int main()
{
    // Change for your location
    char *filename = "/mnt/c/Users/nunon/Desktop/ISEL/SI-2526-PSC/files/frutas.csv";
    struct table *result = table_load_csv(filename);

    return 0;
};