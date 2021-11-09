
//Bibliotecas padrões da linguagem
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Bibliotecas do UNIX
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//Biblioteca Posix pra mostrar o path
#include <unistd.h>

#define PATH_MAX 1024//tamanho maximo da string do path
#define MAX_LINE 100 //tamanho maximo da linha de comando

int main() {

    char comando[MAX_LINE]; //string que recebera o comando
    char **args;            //ponteiro de argumentos
    int argc = 0;           //contador de argumentos
    char cwd[PATH_MAX];     //variavel que contem o path do arquivo

    

    while (1) {

        getcwd(cwd, sizeof(cwd));            //comando getcwd pega o path do arquivo
        printf("%s$ ",cwd);
        fgets(comando, MAX_LINE, stdin);     //recebendo string via teclado
        comando[strlen(comando) - 1] = '\0'; // removendo \n e inserindo o caractere nulo

        if (strcmp(comando, "sair") == 0) exit(0); //se o comando digitado for "sair"

        //tratar os erros
        //criar um processo para cada novo comando
        int pid = fork();

        if (pid == 0 ){ //está no processo filho
            int i = 0;

            for (i = 0; i < strlen(comando); i++) { //conta a quantidade de argumentos

                if (comando[i] == ' ')
                    argc++;
            }

            //aloca vetor de strings baseado na quantidade de argumentos
            args = (char **) malloc((argc + 1) * sizeof(char *));

            //o token será o comando de entrada
            char *token = strtok(comando, " ");

            //inserindo o token na primeira posicao
            args[0] = token;

            i = 1;

            //preenchendo o restante do array de strings com os argumentos
            while (token != NULL) {
                args[i] = strtok(NULL, " ");
                token = strtok(NULL, " ");
                i++;
            }

            //envia argumentos pro exec
            execvp(args[0], args);
        } else if (pid > 0) {
            //espera o processo filho terminar
            //printf("\nProcesso filho finalizou");
            wait(NULL);
        }
    }


}
