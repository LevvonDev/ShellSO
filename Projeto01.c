#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAMANHO_MAX_COMANDO 1024
#define MAX_ARGUMENTOS 64
#define MAX_CAMINHOS 64

char* caminhos[MAX_CAMINHOS]; // Array para armazenar os caminhos definidos

// Protótipos de funções
void analisar_comando(char *comando);
void executar_comando(char *comando);
void encerrar_programa();



void encerrar_programa() {

    pid_t pid;
    int status;


     // Criar um novo processo filho
    pid = fork();

    if (pid < 0) {
        // Erro ao criar o processo filho
        perror("Erro ao criar processo filho");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
           printf("Encerrando o interpretador de comandos...\n");
        exit(EXIT_SUCCESS);
    } else {
        // Código do processo pai
        // Esperar pelo processo filho terminar
        waitpid(pid, &status, 0);
    }

}


int main() {
    char comando[TAMANHO_MAX_COMANDO];

    // Loop principal
    while (1) {
        printf("$ "); // Prompt do shell
        fflush(stdout); // Limpar o buffer de saída
        fgets(comando, TAMANHO_MAX_COMANDO, stdin); // Receber o comando

        // Remover o caractere de nova linha do final do comando
        if (comando[strlen(comando) - 1] == '\n') {
            comando[strlen(comando) - 1] = '\0';
        }

        // Interpretar e executar o comando
        executar_comando(comando);
    }

    return 0;
}

void analisar_comando(char *comando) {
    // Implemente a análise sintática aqui
}


void executar_comando(char *comando) {
    if (strcmp(comando, "exit") == 0) {
        encerrar_programa();
    }
}
