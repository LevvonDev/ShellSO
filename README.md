# Projeto-Final-OS

# Preparações
- Baixe o arquivo desse repositório da branch "main" zipado, ou clone o respositorio na sua pasta compartilhada entre o seu sistema e a VM com o seguinte comando:
```
git clone https://github.com/LevvonDev/ShellSO.git
```
- Antes de compilar e executar o código você deve instalar a biblioteca readline com o seguinte comando dentro da VM:

```
sudo apt-get install libreadline-dev
```
- Agora, compile o código com o seguinte comando dentro da VM:

```
gcc -o shell shell.c -lreadline
```
- Pronto, agora você pode executar o shell com o comando dentro da VM:

```
./shell
```

# Opções de Execução

Quando você executar o shell, você sera perguntado se deseja executar os comandos através do arquivo .batch ou por linhas de código

  - Digite "linha" para executar através de comandos pelo terminal ou "batch" para executar através do arquivo .batch

# Comandos

- Os seguintes comandos estão disponiveis no nosso shell:

  **Comandos Disponiveis**
    - Path: Define caminhos para busca de outros executaveis  **Exemplo**:<path /home/usuario/pasta-compartilhada/ShellSO/ProjetoSO/comandos-externos>
      **IMPORTANTE**
        - Trocar "usuario" pelo seu usuario e "pasta-compartilhada" pela sua pasta compartilhada entre o seu sistema e a VM
    - cd: Muda o diretório atual                              **Exemplo**:<cd nome do diretório>
    - ls: Lista o conteudo do diretorio atual                 **Exemplo**:<ls>
    -  ls -l: Lista informações sobre todos os arquivos      **Exemplo**:<ls -l>
    - ls -a: Lista todos os arquivos do diretorio atual, incluindo arquivos ocultos **Exemplo**:<ls -a>
    - cat: Le o conteudo de um arquivo e escreve a saida no terminal **Exemplo**:<cat ls.c>
    - Redirecionamento de Saida: Voce pode redirecionar a saida dos comandos ls/ls -l/ls -a/ cat para um arquivo saida.txt utilizando o sinal ">" apos o comando 
                              **Exemplo**: "cat ls.c > saida.txt"
                              
  **IMPORTANTE**
   - O comando **PATH** deve ser o primeiro a ser executado independente da versão de execução. Ele será usado para o shell encontrar os outros comandos.

# Batch

- Para executar os comandos através do .batch você deve editar o arquivo shell.batch, você pode utilizar os comandos que ja estão definidos no arquivo como base.

# Linhas

- Para executar os comandos através de linhas no terminal, apenas siga os exemplos do seguimento **Comandos Disponíveis**.
    

  

