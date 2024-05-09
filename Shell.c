#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGUMENTS 64
#define MAX_PATHS 64 


char *paths[MAX_PATHS]; 
int num_paths = 0; 

int parse_command(char *command, char ***arguments);
void free_arguments(char **arguments);
void handle_internal_commands(char **arguments);

int main() {
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
            handle_internal_commands(arguments);
        }

     
        free(command);
        free_arguments(arguments);
        arguments = NULL;

      
        free(input);
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


void handle_internal_commands(char **arguments) {
    if (strcmp(arguments[0], "exit") == 0) {
        printf("SAINDO...\n");
        exit(EXIT_SUCCESS);
    } 
    else if (strcmp(arguments[0], "cd") == 0) {
        if (arguments[1] == NULL) {
            printf("Por favor especifique um diretório\n");
            return;
        } else {
            if (arguments[1] != NULL) {
                chdir(arguments[1]);
            }
        }
    } 
    else if (strcmp(arguments[0], "path") == 0) {
        for (int i = 0; i < num_paths; i++) {
            free(paths[i]);
        }
        num_paths = 0;

        for (int i = 1; arguments[i] != NULL; i++) {
            add_path(arguments[i]);
        }
    }
}
