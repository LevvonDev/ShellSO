#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

void print_file_info(struct dirent *entry) {
    struct stat fileStat;
    if (stat(entry->d_name, &fileStat) < 0) {
        perror("stat");
        return;
    }

    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

    printf(" %ld", fileStat.st_nlink);

    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group *gr = getgrgid(fileStat.st_gid);
    printf(" %s %s", pw->pw_name, gr->gr_name);

    printf(" %5ld", fileStat.st_size);

    char time[20];
    strftime(time, 20, "%b %d %H:%M", localtime(&(fileStat.st_mtime)));
    printf(" %s", time);

    printf(" %s\n", entry->d_name);
}

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *entry;
    int show_all = 0;
    int long_format = 0;

    // Processa os argumentos da linha de comando
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            show_all = 1;
        } else if (strcmp(argv[i], "-l") == 0) {
            long_format = 1;
        }
    }

    // Abre o diretório atual para leitura
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    // Lê e imprime os nomes dos arquivos e diretórios no diretório
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }

        if (long_format) {
            print_file_info(entry);
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    // Fecha o diretório
    closedir(dir);
    return 0;
}
