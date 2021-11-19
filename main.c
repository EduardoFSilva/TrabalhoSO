
//Bibliotecas padrões da linguagem
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
//Bibliotecas do UNIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Biblioteca Posix pra mostrar o path
#include <unistd.h>

#define PATH_MAX 1024//tamanho maximo da string do path
#define MAX_LINE 100 //tamanho maximo da linha de comando
#define READ_END 0 //Ponta de leitura
#define WRITE_END 1 //Ponta de escrita

char **linha_Comando(char *comando, int *arg);

void executar_Comando(char **args, int argP);

char **separar_Args(char **args, int pos);

int main() {

    setbuf(stdout, NULL);
    char comando[MAX_LINE], cwd[PATH_MAX]; //string que recebera o comando e Path
    int argP = 0; //contador de Pipes

    while (1) {

        getcwd(cwd, sizeof(cwd));            //comando getcwd pega o path do arquivo
        printf("\n%s$ ", cwd);
        fgets(comando, MAX_LINE, stdin);     //recebendo string via teclado
        comando[strlen(comando) - 1] = '\0'; // removendo \n e inserindo o caractere nulo

        if (strcmp(comando, "sair") == 0) {
            exit(0); //se o comando digitado for "sair"
        }

        pid_t pid = fork();

        if (pid == 0) { //está no filho, executar o comando

            argP = 0;
            char **linhaComando = linha_Comando(comando, &argP);

            executar_Comando(linhaComando, argP);
            exit(0);

        } else if (pid > 0) { //está no pai, esperar o filho

            wait(NULL);
        } else { //deu ruim

            fprintf(stderr, "\nErro ao alocar processo !");
            exit(1);
        }

    }
    exit(0);
}

//cria o fork e executa o comando
void executar_Comando(char **args, int argP) {

    if (argP == 0) { //nao tem pipe, comando simples

        //separando os argumentos
        args = separar_Args(args, 0);
        //enviando argumentos pro exec
        execvp(args[0], args);
        //caso dê ruim no exec
        perror("\nComando desconhecido");
        exit(1);

    } else { //tem Pipe

        int fd[2];
        if (pipe(fd) == -1) {

            fprintf(stderr, "\nFalha na criacao do pipe()");
            exit(1);
        }

        if(fork()==0){

            close(STDIN_FILENO);
            dup(fd[READ_END]);
            close(fd[READ_END]);
            close(fd[WRITE_END]);

            char** argAux = separar_Args(args,1);

            execvp(argAux[0],argAux);
            perror("\nComando desconhecido");
            exit(1);
        }

        //fechando as saidas,leituras e amarrando com o dup
        close(STDOUT_FILENO);
        dup(fd[WRITE_END]);
        close(fd[WRITE_END]);
        close(fd[READ_END]);

        char** argAux = separar_Args(args,0);

        execvp(argAux[0],argAux);
        perror("\nComando desconhecido");
        exit(1);


    }

}


//monta a linha de comando
char **linha_Comando(char *comando, int *arg) {

    char **args = NULL;
    int argc = 0, i = 0;

    for (i = 0; i < strlen(comando); i++) { //conta a quantidade de pipes
        if (comando[i] == '|')
            argc++;
    }

    if (argc > 0) {

        //aloca vetor de strings baseado na quantidade de argumentos
        args = (char **) malloc((argc + 1) * sizeof(char *));
        //inserindo o token na primeira posicao
        i = 0;
        char *token = strtok(comando, "|");
        //preenchendo o restante do array de strings com os argumentos
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, "|");
            i++;
        }
        args[i] = NULL;

        *arg = argc;

    } else {

        args = (char **) malloc(sizeof(char *));
        args[0] = comando;

    }

    return args;

}

char **separar_Args(char **args, int pos) {

    int argc = 0;
    char *comando = args[pos];
    char **argsAux = NULL;

    for (int i = 0; i < strlen(comando); i++) { //conta a quantidade de argumentos

        if (comando[i] == ' ')
            argc++;

    }

    if (argc > 0) { //significa que tem argumentos para serem separados

        // aloca novo vetor de strings pro tamanho adequado de argumentos
        argsAux = (char **) malloc((argc + 1) * sizeof(char *));

        //hora de quebrar a string e obter os argumentos
        char *token = strtok(comando, " ");
        int i = 0;

        while (token) {

            argsAux[i] = token;
            token = strtok(NULL, " ");
            i++;

        }
        argsAux[i] = NULL; //ultima posicao deve sempre terminar NULL


    } else {

        argsAux = (char **) malloc(sizeof(char **));
        argsAux[0] = args[pos];


    }

    return argsAux;

}