
/*

    4. [3] Implemente a função list_insert_sorted em assembly x86-64.

    struct list_node {
        struct list_node *next, *prev;
        void *data;
    };

    bool list_insert_rear(struct list_node *node, void *data);

    bool list_insert_sorted(struct list_node *list, void *data,int (*compare)(const void*, const void*))
    {
        struct list_node *node;
        for (node = list‐>next; node != list && compare(node‐>data, data) < 0; node = node‐>next);
        return list_insert_rear(node, data);
    }

*/

    .text
    .global list_insert_sorted

//      PARAMETROS
//  %rdi -> *list
//  %rsi -> *data
//  %rdx -> int (*compare)(const void*, const void*) pointer para função
//      VALS
//
//
//
//

list_insert_sorted:
    push %r12
    movq %rdi, %r12     # %r12 = node
    movq (%r12, $0),%r12# %r12 = list->next

for_loop:
    cmp %r12, %rdi      # node != list
    bne return
    push %r10
    movq %rdi, %r10     # temporariamente guardar a lista
    movq (%r12,$16), %rdi   # node‐>data
    call *%rdx          # int (*compare)(const void*, const void*)????
    pop %r10
    movq %r10, %rdi     # devolta para o primeiro param
    cmp %rax, $0        # compare(node‐>data, data) < 0
    bge return
    movq (%r12,$0), $r12
    b for_loop

return:
    movq %r12, %rdi
    call list_insert_rear

end:
    pop %r12
    ret

    .section    .note.GNU-stack


