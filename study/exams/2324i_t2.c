
/*

1. [1,5] Programe em linguagem C a função void set_bits(unsigned long *value, int position, int
size); que afeta com um os bits das posições entre position e position + size - 1 do valor apontado por
value, mantendo o valor das restantes posições

*/

void set_bits(unsigned long *value, int position, int size)
{
    unsigned long mask = 1 << position;
    unsigned long final_mask = 1 << position + size - 1;
    while (mask <= final_mask)
    {
        *value |= mask;
        mask <<= 1;
    }
}

/*

2. [2] Escreva em linguagem C, a função str_to_acronym que substitui a frase contida na string str pelo seu
acrónimo. A frase original pode conter várias palavras, separadas por espaço, tab ou sequências destes
carateres. Considera-se palavra qualquer sequência de caracteres que não sejam separadores. O acrónimo
produzido é a sequência dos caracteres iniciais (das palavras) que sejam letras maiúsculas ou algarismos. A
função retorna a quantidade de carateres do acrónimo.
int str_to_acronym(char *str);
Exemplos: “ALGORITMOS e Estruturas de Dados” -> “AED”; “Arquitetura de COMPUTADORES” -> “AC”;
“REDES de Computadores” -> “RC”; “Optativa 1” -> “O1”; “Optativa 2” -> “O2”;

*/

int str_to_acronym(char *str)
{
    int i = 0;
    int j = 0;
    int at_word_start = 1;

    while (str[i] != '\0')
    {
        if (str[i] == ' ' || str[i] == '\t')
        {
            at_word_start = 1;
        }
        else
        {
            if (at_word_start &&
                ((str[i] >= 'A' && str[i] <= 'Z') ||
                 (str[i] >= '0' && str[i] <= '9')))
            {

                str[j++] = str[i];
            }
            at_word_start = 0;
        }
        i++;
    }

    str[j] = '\0';
    return j;
}

/*
3. [3,5] Implemente a função data_not_equal em linguagem assembly x86-64, cuja definição em linguagem C se
apresenta a seguir.
*/

struct data
{
    char label[10];
    int value;
};
struct set
{
    int id;
    size_t len;
    struct data data[];
};
int data_not_equal(struct set **set1, struct set **set2, size_t set_idx, size_t data_idx)
{
    if (data_idx >= set1[set_idx]‐> len || data_idx >= set2[set_idx]‐> len)
        return 0;
    return set1[set_idx]‐> data[data_idx].value != set2[set_idx]‐> data[data_idx].value;
}

/*
    TAMANHOS

data->label = 10 bytes + 2 de padding para ser multiplo de 4;
data->value = 4 bytes (fica a 16 até aqui)

set->id = 4bytes
set->len = 4bytes
set->data = pointeiro de 8 bytes logo 16;

Isto fica data = 16bytes e set = 16bytes
*/

/*
    VARIAVEIS

%rdi = **set1
%rsi = **set2
%rdx = set_idx
%rcx = data_idx

*/
/*
.global data_not_equal

    data_not_equal:
    push %r12
    push %r13
    push %r14
    // passo 1  set1[set_idx]‐>len
    movq  (%rdi, %rdx, $8), %r12    // set1[set_idx]
    movq  4(%r12), %r13             // set1[set_idx].len
    cmpq %rcx, %r13
    bge second_condition
    movq  $0,   %rax
    b return

second_condition:
    movq  (%rsi, %rdx, $8), %r12    // set2[set_idx]
    movq  4(%r12), %r13             // set2[set_idx].len
    cmpq %rcx, %r13
    bge return_value
    movq $0, %rax
    b return

return_value:
    movq  (%rdi, %rdx, $8), %r12    // set1[set_idx]
    movq  (%rsi, %rdx, $8), %r14    // set2[set_idx]
    movq  0(%r12), %r12             // set1[set_idx].data
    movq  0(%r14), %r14             // set2[set_idx].data
    movq (%r12, %rcx, 1), %r12    // set1[set_idx].data[data_idx]
    movq (%r14, %rcx, 1), %r14    // set2[set_idx].data[data_idx]
    movw 4(%r12), %r12w
    movw 4(%r14), %r14w
    cmpw %r12w, %r14w
    be  equal
    movq $0, %rax
    b return

equal:
    movq    $1, %rax

return:
    pop %r13
    pop %r12
    ret
*/

size_t array_remove_cond(void **array, size_t size,
                         int (*eval)(const void *, const void *), void *context)
{
    for (void **current = array, **last = array + size; current < last;)
    {
        if (eval(*current, context))
        {
            memmove(current, current + 1, (last ‐ current ‐ 1) * sizeof(void *));
            size ‐= 1;
            last ‐= 1;
        }
        else
        {
            current += 1;
        }
    }
    return size;
}

// a.[2] Implemente a função array_remove_cond em assembly x86 - 64.

/*

b. [2,5] Escreva, em linguagem C, um programa de teste da função array_remove_cond. Este programa deve
remover de um array de ponteiros para struct student, os ponteiros que correspondam a estudantes com
número superior ao dado como argumento do executável. No programa, deve constar a definição estática do
array de ponteiros e das instâncias apontadas, a definição da função de verificação e a chamada à função
array_remove_cond.

*/

struct student
{
    char *name;
    int number;
};

int eval(const void *data, const void *context)
{
    const struct student *s = data;
    const int *limit = context;

    return s->number > *limit;
}

int main(int argc, char **argv)
{
    struct student s1 = {"Ana", 1};
    struct student s2 = {"Bruno", 2};
    struct student s3 = {"Carla", 3};

    struct student *array[] = {&s1, &s2, &s3};

    size_t size = 3;

    size = array_remove_cond(array, size, eval, &limit);

    assert(size == 1);
    assert(array[0]->number == 1);

    return 0;
}
