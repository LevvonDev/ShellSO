#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGUMENTS 64

// Funções prototipadas
int parse_command(char *command, char ***arguments);
int execute_command(char **arguments);
void free_arguments(char **arguments);
void handle_internal_commands(char **arguments);

int main() {
    char *command = NULL;
    char **arguments = NULL;
    int num_tokens;

    while (1) {
        // Obtém o diretório de trabalho atual
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Shell (%s)> ", cwd);
        } else {
            perror("getcwd() error");
            break;
        }

        // Lê a entrada do usuário usando readline
        char *input = readline("");

        // Se a entrada for NULL, o usuário pressionou Ctrl+D, então saímos do loop
        if (input == NULL) {
            printf("\n");
            break;
        }

        // Adiciona a entrada ao histórico do readline
        if (strlen(input) > 0) {
            add_history(input);
        }

        // Analisa o comando e obtém os argumentos
        num_tokens = parse_command(input, &arguments);

        // Aloca dinamicamente memória para o comando
        command = strdup(input);
        if (command == NULL) {
            perror("Falha na alocação de memória para o comando");
            exit(EXIT_FAILURE);
        }

        // Verifica se há comandos internos
        if (num_tokens > 0) {
            handle_internal_commands(arguments);
        }

        // Libera a memória alocada para o comando e os argumentos
        free(command);
        free_arguments(arguments);
        arguments = NULL;

        // Libera a memória alocada para a entrada do readline
        free(input);
    }

    return 0;
}

int parse_command(char *command, char ***arguments) {
    int num_tokens = 0;
    char *token;
    char **args = NULL;

    // Aloca dinamicamente memória para o array de argumentos
    args = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
    if (args == NULL) {
        perror("Falha na alocação de memória para os argumentos");
        exit(EXIT_FAILURE);
    }

    // Divide o comando em tokens usando strtok
    token = strtok(command, " ");
    while (token != NULL) {
        // Aloca dinamicamente memória para cada token
        args[num_tokens] = strdup(token);
        if (args[num_tokens] == NULL) {
            perror("Falha na alocação de memória para os argumentos");
            exit(EXIT_FAILURE);
        }

        // Incrementa o contador de tokens
        num_tokens++;

        // Obtém o próximo token
        token = strtok(NULL, " ");
    }

    // Define o último elemento do array de argumentos como NULL
    args[num_tokens] = NULL;

    // Atualiza o ponteiro de argumentos
    *arguments = args;

    // Retorna o número de tokens
    return num_tokens;
}

int execute_command(char **arguments) {
    // Implementação futura
    printf("Comando executado com sucesso!\n");
    return 0;
}

void free_arguments(char **arguments) {
    // Libera a memória alocada para cada argumento e o array de argumentos
    if (arguments != NULL) {
        for (int i = 0; arguments[i] != NULL; i++) {
            free(arguments[i]);
        }
        free(arguments);
    }
}

void handle_internal_commands(char **arguments) {
    if (strcmp(arguments[0], "exit") == 0)
    {
        printf("SAINDO...\n");
        // Saindo do shell
        exit(EXIT_SUCCESS);
    }

    else if (strcmp(arguments[0], "cd") == 0) {
        
        if (arguments[1] == NULL)
        {
            printf("Por favor especifique um diretorio\n");
            return;
        } 
        else 
        {
            if (arguments[1] != NULL) 
            {
                chdir(arguments[1]);
                
            }
        }
    }
}
