/*
1. [1,5] Programe em linguagem C a função int make_mask(int position, int size); que devolve um valor
inteiro com 0 nas posições entre position e position + size ‐ 1 e 1 nas restantes posições.

*/

int make_mask(int position, int size)
{
    int return_value = 0;
    int mask = 1;
    for (int i = 0; i < 32; i++)
    {
        // Condição de 0
        if (i >= position && i <= position + size - 1)
        {
            return_value &= ~mask;
        }
        else
        {
            // Condição de 1
            return_value |= mask;
        }
        mask <<= 1;
    }
    return return_value;
}

/*

2. [2,0] Escreva a função strcasecmp, que compara letra-a-letra. no sentido de endereços crescentes, as strings
str1 e str2, ignorando a forma maiúscula ou minúscula das letras. Retorna um valor negativo se na primeira
diferença a letra de str1 for de ordem inferior à da letra de str2, um valor positivo no caso contrário ou zero
se ambas as strings forem iguais.

*/

int strcasecmp(const char *str1, const char *str2)
{
    // Teoricamente ambas a Strings tem o mesmo tamanho
    for (int i = 0; str1[i] != '\0'; i++)
    {
        if (str1[i] - str2[i] > 0)
            return 1;
        else
            return -1;
    }
    // Caso de serem iguais
    return 0;
}

typedef struct list_node
{
    void *data;
    struct list_node *next;
} List_node;

List_node *list_search(List_node *sentinel,
                       int (*predicate)(const void *, const void *), const void *context)
{
    for (List_node *node = sentinel‐ > next; node != sentinel; node = node‐ > next)
        if (predicate(node‐ > data, context))
            return node;
    return NULL;
}

/*
b. [2,5] Escreva, em linguagem C, um programa de teste da função list_search para verificar se um
estudante com o nome passado como argumento ao executável, faz parte de uma lista de informação
sobre estudantes, com elementos do tipo struct student. No programa, deve constar a definição
estática de uma lista com pelo menos dois estudantes, a definição da função de verificação, a chamada à
função list_search, assim como o código para apresentação do resultado.
struct student { char *name; int number; };
*/

struct student
{
    char *name;
    int number;
};

int check_student(const void *data, const void *context)
{
    const struct student *st = (const struct student *)data;
    const char *target_name = (const char *)context;
    return strcmp(st->name, target_name) == 0;
}

int main()
{
    struct student target = {"Ana", 1001};

    struct student a = {"Ana", 1001};
    struct student b = {"Bruno", 1002};

    List_node sentinel = {.data = NULL, .next = NULL}; /* nó sentinela */
    List_node n1 = {.data = &a, .next = NULL};
    List_node n2 = {.data = &b, .next = NULL};

    sentinel.next = &n1;
    n1.next = &n2;
    n2.next = &sentinel;

    List_node *found = list_search(&sentinel, check_student, target);

    assert(found != 0);
}

1024 512 256 128 64 32 16 8 4 2 1

    /*
    7. [5] Programe em linguagem C as funções filepath_split e filepath_build. Ambas as funções devem
    alocar memória para suportar as estruturas de dados criadas e libertar a memória associada às estruturas de
    dados recebidas.
    */

    /*
    a) [2,5] A função filepath_split recebe um caminho para ficheiro e divide-o em componentes. As
    componentes são copiadas para strings autónomas e registadas, por ordem, num array de ponteiros para
    string. O parâmetro separator indica o caractere separador (no caso do UNIX seria o caractere ‘/’). O
    parâmetro filepath é uma string C contendo o caminho completo. O parâmetro comps_size é um
    parâmetro de saída, para se indicar a dimensão do array de ponteiros criado – corresponde ao número de
    componentes do caminho. A função devolve um ponteiro para o array de ponteiros criado.
    */

    struct storage
{
    char *data;
};

char **filepath_split(char separator, char *filepath, size_t *comps_size)
{
}

/*

7. [5] O tipo Fifo representa uma sequência de objetos genéricos, internamente organizados em lista simplesmente
ligada. Os ponteiros begin e end delimitam a lista, de modo a permitir a inserção de objetos num extremo e a
remoção de objetos pelo outro extremo a custo unitário. O campo size indica o número de elementos na lista. Os
nós da lista são instâncias do tipo Node, em que o campo next indica o próximo nó e o campo data aponta o
objeto genérico armazenado.

a. [3] Implemente a função fifo_create_from_array que cria uma instância de tipo Fifo, representando o
conjunto de objetos indicados pelo array object_array e dimensão object_array_size.

*/

typedef struct node
{
    struct node *next;
    void *data;
} Node;

typedef struct fifo
{
    size_t size;
    Node *begin, *end;
} Fifo;

/*
Antes de começar a escrever

Vou ter malloc para fifo

*/

Fifo *fifo_create_from_array(void **object_array, size_t object_array_size)
{
    struct fifo list = malloc(object_array_size * sizeof(struct node));
    list->size = object_array_size;

    struct node lastNodeNext = NULL;

    for (int i = 0; i < (int)object_array_size; i++)
    {
        struct node currentNode = malloc(sizeof(Node));
        if (i == 0)
        {
            list->begin = *currentNode;
        }

        if (lastNodeNext != NULL)
        {
            lastNodeNext->next = currentNode;
        }

        currentNode->data = object_array[i];
        list->end = *currentNode;
        lastNodeNext = currentNode
    }
    return list;
}

/*
b. [2] Implemente a função fifo_destroy que liberta os recursos de memória alocados na função anterior.
*/

void fifo_destroy(Fifo *fifo)
{
    // Primeiro objetivo é conseguir chegar ao primeiro elemento para começar a usar nexts
    // Dar free a cada nó e seguir para o próximo
    // Dar free á struct fifo no final
    struct Node *node = fifo->begin;
    while (node != NULL)
    {
        Node *current_node = node->next;
        free(node);
        node = current_node;
    }
    free(*fifo);
}
