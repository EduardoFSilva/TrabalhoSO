
//Bibliotecas padrões da linguagem
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
int contar_Pipes(char **comando, int argc);
void executar_Comando(int qtdPipes,char** args,int argc);

int main() {

    char comando[MAX_LINE], cwd[PATH_MAX]; //string que recebera o comando e Path
    int argc = 0;
    while (1) {

        getcwd(cwd, sizeof(cwd));            //comando getcwd pega o path do arquivo
        printf("%s$ ", cwd);
        fgets(comando, MAX_LINE, stdin);     //recebendo string via teclado
        comando[strlen(comando) - 1] = '\0'; // removendo \n e inserindo o caractere nulo

        if (strcmp(comando, "sair") == 0) exit(0); //se o comando digitado for "sair"

        //tratar os erros
        //criar um processo para cada novo comando

         int pid = fork();
         assert(pid > 0 || pid == 0); //vai assegurar de que o processo tenha sido alocado com sucesso

         if (pid == 0) { //está no processo filho

             argc = 0; //a cada novo processo, o numero de argumentos deve ser zerado
             char **args = linha_Comando(comando, &argc); //montando a linha de comando e obtendo o numero de argumentos

             int qtd_Pipes = contar_Pipes(args, argc); //obtendo a quantidade de pipes a ser feita

             if(qtd_Pipes) //é um comando com pipe
                 executar_Comando(qtd_Pipes,args,argc); //vai pra outra subrotina
             else{ //é um comando simples

                 //enviando argumentos pro exec
                 int error = execvp(args[0], args);

                 if (error == -1) { //funcao execvp() retorna -1 em caso de erro

                     fprintf(stderr, "Comando desconhecido\n");
                     exit(1);
                 }

             }



         } else if (pid > 0) {

             //espera o processo filho terminar
             wait(NULL);
         } else {

             fprintf(stderr, "Erro ao alocar processo !");
             exit(1);

         }

        }

    }
    void executar_Comando(int qtdPipes,char** args,int argc){






}

    int contar_Pipes(char **comando, int argc) {

        int pipes = 0;

        for (int i = 0; i < argc + 1; i++) {

            if (!strcmp(comando[i], "|"))
                pipes++;

        }

        return pipes;

    }

    char **linha_Comando(char *comando, int *arg) {

        char **args = NULL;
        int argc = 0, i = 0;

        for (i = 0; i < strlen(comando); i++) { //conta a quantidade de argumentos
            if (comando[i] == ' ')
                argc++;
        }
        //aloca vetor de strings baseado na quantidade de argumentos
        args = (char **) malloc((argc + 1) * sizeof(char *));
        //inserindo o token na primeira posicao
        i = 0;
        char *token = strtok(comando, " ");
        //preenchendo o restante do array de strings com os argumentos
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;


        *arg = argc;
        return args;

    }
