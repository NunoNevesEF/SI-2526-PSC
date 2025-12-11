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

/*c. Programe a função table_save_csv que escreve em ficheiro, em formato CSV, o conteúdo de informação
de uma tabela struct table.
void table_save_csv(const struct table *table, const char *filename);*/

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

/*e. Programe a função table_filter que cria uma nova tabela com as linhas que satisfazem um predicado. O
parâmetro predicate recebe o ponteiro para a função onde é programada a verificação do predicado; o
parâmetro context recebe dados de contexto necessários à verificação.
struct table *table_filter(const struct table *table,
bool (*predicate)(const void *row, const void *context),
const void *context); */

/**
 * @Param *table, non filtered table
 * @Param bool, function predicado(const void *row, const void *context)
 * @Param *context, criteria being used
 */
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

/*f. Para teste da função table_filter realize um programa que recebendo o ficheiro frutas.csv produza
um novo ficheiro com os frutos de preço inferior a um Euro.*/
void testing_filter_csv(char *inFile, char *outFile)
{
    int limit = 1;
    struct table *result = table_load_csv(inFile);
    struct table *res = table_filter(result, price_lower_than_euro, &limit);
    table_save_csv(res, outFile);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Wrong number of arguments, please try again...\n");
        return 1;
    }

    char *inFile = argv[1];
    char *outFile = argv[2];

    testing_filter_csv(inFile, outFile);

    return 0;
};