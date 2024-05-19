#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: cat <arquivo>\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            perror("Erro ao abrir o arquivo");
            continue;
        }

        char ch;
        while ((ch = fgetc(file)) != EOF) {
            putchar(ch);
        }

        putchar('\n');
        
        fclose(file);
    }

    return 0;
}
