#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>
#include <csv.h>

// ./ex2_filter frutas.csv frutas_filter_output.csv

// ./ex2_filter gutenberg_metadata.csv gutenberg_filter_output.csv


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

    struct
    {
        bool cols_done;
        char **current_row;
        size_t current_col;
    } state;
};

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

/*b. Programe a função table_load_csv que cria uma tabela (uma instância de struct table) preenchida com
os dados lidos de um ficheiro em formato CSV. Considere que o ficheiro está sempre bem formado.
struct table *table_load_csv(const char *filename);*/


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

/*c. Programe a função table_save_csv que escreve em ficheiro, em formato CSV, o conteúdo de informação
de uma tabela struct table.
void table_save_csv(const struct table *table, const char *filename);*/


static void write_csv_field(FILE *f, const char *s)
{
    if (!s) s = "";
    bool need_quote = strchr(s, ',') || strchr(s, '\n') || strchr(s, '"');

    if (need_quote) fputc('"', f);

    for (const unsigned char *p = (const unsigned char *)s; *p; ++p)
    {
        if (*p == '"')
        {
            fputc('"', f);
            fputc('"', f);
        }
        else
        {
            fputc(*p, f);
        }
    }

    if (need_quote) fputc('"', f);
}

void table_save_csv(const struct table *table, const char *filename)
{
    char fileLocation[256] = "generatedFiles/";
    strncat(fileLocation, filename, sizeof(fileLocation) - strlen(fileLocation) - 1);
    FILE *fptr = fopen(fileLocation, "w");
    if (!fptr) return;

    // cabeçalho
    for (int c = 0; c < table->nCols; c++)
    {
        write_csv_field(fptr, table->columns[c]);
        if (c + 1 < table->nCols) fputc(',', fptr);
    }
    fputc('\n', fptr);

    // linhas
    for (int r = 0; r < table->nRows; r++)
    {
        for (int c = 0; c < table->nCols; c++)
        {
            write_csv_field(fptr, table->rows[r][c]);
            if (c + 1 < table->nCols) fputc(',', fptr);
        }
        fputc('\n', fptr);
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

bool number_lower_than_given(const void *row, const void *context)
{
    char **linha = (char **)row;
    int limit = *(int *)context;

    int number = atoi(linha[0]);

    return number < limit;
}

/*f. Para teste da função table_filter realize um programa que recebendo o ficheiro frutas.csv produza
um novo ficheiro com os frutos de preço inferior a um Euro.*/
void testing_filter_csv(char *inFile, char *outFile)
{
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