#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }
    char *p = argv[1];

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("  mov rax, %ld\n", strtol(p, &p, 10));
    while (*p) {
        switch (*p) {
            case '+':
                printf("  add rax, %ld\n", strtol(p + 1, &p, 10));
                break;
            case '-':
                printf("  sub rax, %ld\n", strtol(p + 1, &p, 10));
                break;
            default:
                fprintf(stderr, "無効な式です\n");
                return 1;
        }
    }
    printf("  ret\n");
    return 0;
}