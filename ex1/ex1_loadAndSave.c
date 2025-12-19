#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>
#include <csv.h>

// ./ex1_loadAndSave frutas.csv frutas_copy_output.csv
// ./csv_compare frutas.csv generatedFiles/frutas_copy_output.csv

// ./ex1_loadAndSave gutenberg_metadata.csv gutenberg_copy_output.csv
// ./csv_compare gutenberg_metadata.csv generatedFiles/gutenberg_copy_output.csv




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


