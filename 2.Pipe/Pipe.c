#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#define ERRO               1

/* Implemente o programa Produtor-Consumidor como vimos em aula com dois processos que
utilizam pipes (anonymous pipes, para ser mais preciso) para fazer a comunica ̧c ̃ao. 
O programa
produtor deve gerar n ́umeros inteiros aleat ́orios e crescentes, da seguinte forma: Ni = Ni−1 + ∆,
onde N0 = 1 e ∆  ́e um valor aleat ́orio entre 1 e 100. 

O programa consumidor deve receber o
n ́umero e verificar se o mesmo  ́e primo, imprimindo o resultado no terminal. 

Seu programa
deve primeiramente criar um pipe e depois fazer um fork() para duplicar o processo, de forma
que os dois processos (pai e filho) tenham as duas duas respectivas pontas do pipe (write end
e read end). 

O processo consumidor deve terminar quando receber o n ́umero 0. O programa produtor tem como 
parˆametro o n ́umero de n ́umeros a serem gerados (ex. 1000), depois do qual
o n ́umero zero  ́e enviado, e o produtor termina execu ̧c ̃ao.
Cuidado com a representa ̧c ̃ao num ́erica atrav ́es do pipe! Dica: converta o n ́umero para uma
string de tamanho fixo, ex. 20 bytes. Escreva e leia do pipe este mesmo n ́umero de bytes, para
cada mensagem. Teste o seu programa mostrando seu funcionamento para alguns casos

*/

bool Primo(int numero){
     if (numero <= 1) return false;
     if (numero % 2 == 0 && numero > 2) return false;
     for(int i = 3; i < numero / 2; i+= 2)
     {
         if (numero % i == 0)
             return false;
     }
     return true;
}


void Produtor(){

}


int Consumidor(){

}

int main(){

//fork cria dois processos e cada processo roda uma função

    pid_t pid;

    int descritoresPipe[2], numeroAleatorio=0;
    //descritoresPipe[0] - leitura do pipe
    //descritoresPipe[1] - escrita no pipe

    //Criando o pipe e colocando em um if para caso gere erro
    if(pipe(descritoresPipe)){
        printf ("Falha na criação do Pipe.\n");
        return ERRO;
    }

    //Para gerar um novo processo é realizado um fork
    //if para detectar falhas

    pid = fork ();
    
    if (pid < 0){
        printf ("Falha no fork.");
        return ERRO;
    }

    //Trecho que o filho executa
    else if(pid==0){

        srand(time(NULL));
        numeroAleatorio = rand() % 100;
        printf("O número Aleatório gerado é %d \n", numeroAleatorio);
        
        //Escreve para o pai o número gerado informando seu tamanho
        write (descritoresPipe[1],&numeroAleatorio, sizeof(numeroAleatorio));

        //Processo filho fecha ponta de leitura
        close (descritoresPipe[0]);

        _exit(0);

    }
    //Trecho que o pai executa
    else{

        // Proceso pai fecha ponta aberta do pipe
        close (descritoresPipe[1]);

        // Lê o que filho escreveu através do pipe
        read(descritoresPipe[0], &numeroAleatorio, sizeof(numeroAleatorio));
        //*comandoParaExecutar=numeroAleatorio;

        printf("O número Aleatório lido é %d \n", numeroAleatorio);

        wait(NULL); //esperando filho
        return 0;
    }



}