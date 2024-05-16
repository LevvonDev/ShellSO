#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

#define MAX_ARGUMENTS 64
#define MAX_PATHS 64

char *paths[MAX_PATHS];
int num_paths = 0;

int parse_command(char *command, char ***arguments);
void free_arguments(char **arguments);
int handle_internal_commands(char **arguments);
int handle_external_commands(char **arguments);

int main() {
    char input[10];

    printf("Deseja executar comandos por batch ou por linhas de comando? (batch/linha): ");
    fflush(stdin);
    fgets(input, sizeof(input), stdin);

    if (strcmp(input, "batch\n") == 0) {
        FILE *batch_file;
        char *batch_line = NULL;
        size_t len = 0;
        ssize_t read;

        batch_file = fopen("shell.batch", "r");
        if (batch_file == NULL) {
            perror("Erro ao abrir o arquivo .batch");
            exit(EXIT_FAILURE);
        }

        while ((read = getline(&batch_line, &len, batch_file)) != -1) {
            if (batch_line[read - 2] == '\r')
            {
                batch_line[read - 2 ] = '\0';
                batch_line[read - 1] = '\0'; 
            }
            
            if (batch_line[read - 1] == '\n') {
                batch_line[read - 1] = '\0';
            }

            char **arguments = NULL;
            int num_tokens;

            num_tokens = parse_command(batch_line, &arguments);

            // verifica se o comando interno existe
            if (num_tokens > 0) {
                if (!handle_internal_commands(arguments)) {
                    // Se não for um comando interno, verifica se é externo
                    if (!handle_external_commands(arguments)) {
                    }
                }
            }

            free_arguments(arguments);
        }

        free(batch_line);
        fclose(batch_file);
    } else if (strcmp(input, "linha\n") == 0) {
        char *command = NULL;
        char **arguments = NULL;
        int num_tokens;

        while (1) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Shell (%s)> ", cwd);
            } else {
                perror("getcwd() error");
                break;
            }

            char *input = readline("");

            if (input == NULL) {
                printf("\n");
                break;
            }

            if (strlen(input) > 0) {
                add_history(input);
            }

            num_tokens = parse_command(input, &arguments);

            command = strdup(input);
            if (command == NULL) {
                perror("Falha na alocação de memória para o comando");
                exit(EXIT_FAILURE);
            }

            // verifica se o comando interno existe
            if (num_tokens > 0) {
                if (!handle_internal_commands(arguments)) {
                    // Se não for um comando interno, verifica se é externo
                    if (!handle_external_commands(arguments)) {
                    }
                }
            }

            free(command);
            free_arguments(arguments);
            arguments = NULL;

            free(input);
        }
    } else {
        printf("Opção inválida.\n");
        return EXIT_FAILURE;
    }

    return 0;
}

int parse_command(char *command, char ***arguments) {
    int num_tokens = 0;
    char *token;
    char **args = NULL;

    args = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
    if (args == NULL) {
        perror("Falha na alocação de memória para os argumentos");
        exit(EXIT_FAILURE);
    }

    token = strtok(command, " ");
    while (token != NULL) {
        args[num_tokens] = strdup(token);
        if (args[num_tokens] == NULL) {
            perror("Falha na alocação de memória para os argumentos");
            exit(EXIT_FAILURE);
        }

        num_tokens++;
        token = strtok(NULL, " ");
    }

    args[num_tokens] = NULL;
    *arguments = args;

    return num_tokens;
}

void free_arguments(char **arguments) {
    if (arguments != NULL) {
        for (int i = 0; arguments[i] != NULL; i++) {
            free(arguments[i]);
        }
        free(arguments);
    }
}

void add_path(char *path) {
    if (num_paths < MAX_PATHS) {
        paths[num_paths++] = strdup(path);
    } else {
        printf("Limite máximo de paths alcançado.\n");
    }
}

int handle_internal_commands(char **arguments) {
    if (strcmp(arguments[0], "exit") == 0) {
        printf("SAINDO...\n");
        exit(EXIT_SUCCESS);
    } else if (strcmp(arguments[0], "cd") == 0) {
        if (arguments[1] == NULL) {
            printf("Por favor especifique um diretório\n");
            return 1;
        } else {
            if (arguments[1] != NULL) {
                int verifCD = chdir(arguments[1]);
                if (verifCD == -1) {
                    printf("diretorio nao encontrado!\n");
                }
            }
        }
        return 1; // Indicando que o comando interno foi tratado
    } else if (strcmp(arguments[0], "path") == 0) {
        for (int i = 0; i < num_paths; i++) {
            free(paths[i]);
        }
        num_paths = 0;

        for (int i = 1; arguments[i] != NULL; i++) {
            add_path(arguments[i]);
        }
        return 1; // Indicando que o comando interno foi tratado
    }
    return 0; // Não é um comando interno
}

int handle_external_commands(char **arguments) {
    char *command = arguments[0];
    int command_found = 0;

    // verificar se é um comando interno
    if (strcmp(command, "exit") == 0 || strcmp(command, "cd") == 0 || strcmp(command, "path") == 0) {
        return 0;
    }

    for (int i = 0; i < num_paths; i++) {
        // constroi o caminho completo para o comando
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", paths[i], command);

        // verifica se o arquivo é executável
        if (access(full_path, X_OK) == 0) {
            command_found = 1;
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                return 1;
            } else if (pid == 0) {
                execv(full_path, arguments);
                perror("execv");
                exit(EXIT_FAILURE);
            } else {
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    int exit_status = WEXITSTATUS(status);
                }
                return 0;
            }
        }
    }

    if (!command_found) {
        printf("comando não encontrado nos paths definidos!\n");
        return 1;
    }
    return 0;
}
