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

/*d. Para teste das funções anteriores realize um programa que crie uma réplica de um ficheiro CVS. O
programa tem dois parâmetros: o ficheiro criado e o ficheiro fonte.*/

void testing_load_and_save_csv(char *inFile, char *outFile)
{
    struct table *result = table_load_csv(inFile);
    table_save_csv(result, outFile);
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

    testing_load_and_save_csv(inFile, outFile);

    return 0;
};