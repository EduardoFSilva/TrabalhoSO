
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
int *posicoes_pipes(char **comando, int argc);
void executar_Comando(int qtdPipes,char** args,int argc);


int main() {

    setlocale(LC_ALL,"Portuguese");        //comando q3ue possibilita a utilização de sinais gráficos da língua portuguesa

    int fd[2];//pipe 1
    int fd2[2];//pipe 2
    int *verpipe;
    char comando[MAX_LINE], cwd[PATH_MAX]; //string que recebera o comando e Path

    int argc = 0;       //contador de argumentos

    while (1) {

        getcwd(cwd, sizeof(cwd));            //comando getcwd pega o path do arquivo
        printf("%s$ ", cwd);
        fgets(comando, MAX_LINE, stdin);     //recebendo string via teclado
        comando[strlen(comando) - 1] = '\0'; // removendo \n e inserindo o caractere nulo

        if (strcmp(comando, "sair") == 0) 
        {
            exit(0); //se o comando digitado for "sair"
        }


        verpipe= posicoes_pipes(comando);        //percorre a string e verifica se tem algum pipe e recolhe suas posições, se houverem

        if (verpipe!=NULL)
        {
            //função ping pong
        }

        // Se não tiver nenhum pipe, o programa já executa o comando direto
        else
        {
            executar_Comando;
        }

    }

//cria o fork e executa o comando
void executar_Comando(int qtdPipes,char** args,int argc)
{
        //tratar os erros
        //criar um processo para cada novo comando
        int pid = fork();
        assert(pid > 0 || pid == 0); //vai assegurar de que o processo tenha sido alocado com sucesso

        if (pid == 0) { //está no processo filho

            char **args = linha_Comando(comando);

            //enviando argumentos pro exec
            int error = execvp(args[0], args);

            if (error == -1) { //funcao execvp() retorna -1 em caso de erro

                fprintf(stderr, "Comando desconhecido\n");
                exit(1);
            }

        } else if (pid > 0) {

            //espera o processo filho terminar
            wait(NULL);
        } 

        exit(0);
}

//contar quantos pipes tem
int *posicoes_pipes(char **comando, int argc) {
        
        int posicoes[argc];
        int indice;

        indice=0;

        for (int i = 0; i < argc + 1; i++) {
            if (!strcmp(comando[i], "|"))
                pipes++;
                posicoes[indice]=i;
                indice++;
        }

        if(pipes=0)
        {
            return NULL;
        }

        else
        {
            return posicoes;
        }
    }

//monta a linha de comando
char **linha_Comando(char *comando, int *arg) 
{

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
