
/*
3. [3,5] Implemente em assembly x86-64 a função compare_offset, cuja definição em linguagem C se apresentaa seguir.
*/

struct values {
    char len;
    short base;
    short *offset;
};

int compare_offset(struct values values[][4], int i, int j, int k, struct values *avalue) {
    return k < values[i][j].len &&values[i][j].offset[k] == avalue‐>offset[k];
}

/*
Tamanho da Struct

char -> 1 byte
short -> 2 bytes
shot *offset -> ponteiro sempre 8 bytes ( 64 bits )

struct em total fica 1 + 2 + 8 Aproximadamente igual a 16 bytes por causa do padding

Variaveis

%rdi = struct values values[][4],
%rsi = i
%rdx = j
%rcx = k
%r8 = struct values *avalue
*/

.text
.global compare_offset

compare_offset:
    push    %r12
    push    %r13
    leaq   (%rdi, %rsi, $64), %r12  # %r12 = values[i]  Queremos endereço não o conteudo
    movq    (%r12, %rdx, $16), %r13 # %r13 = values[i][j] 
    movq    0(%r12), %rsi       # i não é mais usado por isso %rsi = values[i][j].len
    movq    8(%r12), %rdx       # j não é mais usado por isso %rdx = values[i][j].offset
    cmpq    %rcx, %rdx
    bge     return_false
    movq    (%rdx, %rcx, $8), %rdx  # rdx = values[i][j].offset[k]
    movq    8(%r8), %r13        # %r13 = avalue->offset
    movq    (%r13, %rcx, $8), %r13
    cmpq    %rdx, %r13
    bne     return_false
    movq    $1, %rax
    b return

return _false:
    movq    $0, %rax

return:
    pop     %r13
    pop     %r12
    ret

