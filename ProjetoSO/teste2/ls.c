#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *entry;

    // Abre o diretório atual para leitura
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    // Lê e imprime os nomes dos arquivos e diretórios no diretório
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Fecha o diretório
    closedir(dir);
    return 0;
}
