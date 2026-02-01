/*

1. [1,5] Programe em linguagem C a função int first_leading_one(unsigned int value) que retorna a
posição do primeiro bit a 1, começando na posição de maior peso. Exemplo: first_leading_one(0b10100)
retorna o valor 4.

*/

int first_leading_one(unsigned int value)
{
    int return_idx = 0;
    int i = 0;
    while (value)
    {
        if (value & 1)
        {
            return_idx = i;
        }
        value >>= 1;
        i++;
    }
    return return_idx;
}

/*

2. [1,5] Escreva em linguagem C, a função void strinsert(char *dst, size_t pos, const char *src) que
insere a string apontada por src na string apontada por dst, a partir do índice pos. O conteúdo original da string
dst, a partir dessa posição, deve ser deslocado no sentido do fim da string, o número de posições da dimensão
da string src. No caso de pos indicar um índice além do fim da string dst, a inserção deve ser feita a seguir à
última posição

*/

#include <string.h>

void strinsert(char *dst, size_t pos, const char *src)
{
    size_t len_dst = strlen(dst);
    size_t len_src = strlen(src);

    if (pos > len_dst)
        pos = len_dst;

    memmove(dst + pos + len_src,
            dst + pos,
            len_dst - pos + 1);

    // copiar src para o espaço aberto
    memcpy(dst + pos, src, len_src);
}

/*
movq 8(%rdi), %r12        # r12 = train->cars
movzbl %sil, %esi         # garantir position como inteiro
imul $12, %rsi, %rsi      # rsi = position * sizeof(struct car)
addq %rsi, %r12           # r12 = &cars[position]
movl 8(%r12), %r13d       # r13d = cars[position].capacity
*/

size_t copy_if_ptr(void **dst, void **src, size_t src_size,
                   bool (*predicate)(const void *, const void *), const void *context)
{
    void **dst_iter = dst;
    void **last = src + src_size;
    for (void **src_iter = src; src_iter < last; src_iter++)
        if (predicate(*src_iter, context))
            *dst_iter++ = *src_iter;
    return dst_iter ‐ dst;
}

/*
Aqui o problema foi mais perceber que tinha de desresferenciar em certas linhas por exemplo
    *dst_iter++ = *src_iter;

*/

/*
Escrever uma teste para isto
*/

size_t copy_if_ptr(void **dst, void **src, size_t src_size,
                   bool (*predicate)(const void *, const void *), const void *context);

bool positive(const void *ptr, const void *ctx)
{
    int val = *(int *)ptr;
    return val > 0;
}

int main()
{
    int a = 10, b = -5, c = 27;
    void *src[] = {&a, &b, &c}; // array de ponteiros para int~
    void *dst[3];               // tamanho >= src_size

    size_t n = copy_if_ptr(dst, src, 3, positive, NULL);
    assert(n > 0);
}

/*

Erro de linker quando são 2 ou mais declarções e os tipos não baterem o erro não é tipos errados, é apenas redeclaração.

*/

struct string_array
{
    char **data;
    size_t size;
    size_t capacity;
};

void append(struct string_array *array, const char *string)
{
    if (array->size >= array->capacity)
    {
        // redimensionar o array de ponteiros (dobrar a capacidade por exemplo)
        size_t new_capacity = array->capacity == 0 ? 1 : array->capacity * 2;
        char **new_data = realloc(array->data, new_capacity * sizeof(char *));
        array->data = new_data;
        array->capacity = new_capacity;
    }
    // copiar a string
    array->data[array->size] = strdup(string);
    array->size++;
}

/*
b. [2] A função copy_n cria um novo array dinâmico preenchido com as n
strings presentes no array original a partir do índice p. O novo array deve ser
independente do array original. Implemente esta função em linguagem C.
*/

#include <stdlib.h>
#include <string.h>

struct string_array *copy_n(struct string_array *array, unsigned n, unsigned p)
{
    struct string_array *new_array = malloc(sizeof(struct string_array));

    if (p >= array->size)
        n = 0;
    else if (p + n > array->size)
        n = array->size - p;

    new_array->data = malloc(n * sizeof(char *));

    for (unsigned i = 0; i < n; i++)
    {
        new_array->data[i] = strdup(array->data[p + i]);
    }

    new_array->size = n;
    new_array->capacity = n;

    return new_array;
}

/*

c. [2] A função delete_n elimina até n strings presentes no array a partir do índice p. Se restarem strings
subsequentes, estas devem ser deslocadas n posições para trás.

*/

#include <stdlib.h>

void delete_n(struct string_array *array, unsigned n, unsigned p)
{
    if (p >= array->size)
        return;

    if (p + n > array->size)
        n = array->size - p;

    for (unsigned i = 0; i < n; i++)
    {
        free(array->data[p + i]);
    }

    for (unsigned i = p + n; i < array->size; i++)
    {
        array->data[i - n] = array->data[i];
    }

    array->size -= n;
}
