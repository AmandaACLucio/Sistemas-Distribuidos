#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h> 
#define ERRO               1
#define BUFFER_TAMANHO     20

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

int Delta(){
    return rand()%100+1;
}

int Produtor(int Pipe[], int totalProdutos){

    //Pipe -- 0-read -- 1-write


    char Msg_Produtor[BUFFER_TAMANHO];
    int qntProdutos = 1;

	close(Pipe[0]);  //fechando ponta de leitura                       
	while(totalProdutos > 0)
	{
    

		printf("Produtor escrevendo %d\n", qntProdutos);
		sprintf(Msg_Produtor, "%d", qntProdutos);
        qntProdutos += Delta();
		write(Pipe[1], &Msg_Produtor, sizeof(Msg_Produtor));
		totalProdutos--;
		
	}

    
	sprintf(Msg_Produtor, "%d", 0);
	write(Pipe[1], &Msg_Produtor, sizeof(Msg_Produtor));   
	close(Pipe[1]);//fechando ponta de escrita

	wait(NULL);                                       
    return 0;

}


int Consumidor(int Pipe[]){

    close(Pipe[1]);

    char Msg_Consumidor[BUFFER_TAMANHO];
    int recebido = 0;
    int executando = 1;

    while(executando){


        read(Pipe[0], &Msg_Consumidor, sizeof(char)*BUFFER_TAMANHO);
        recebido = atoi(Msg_Consumidor);

        if(recebido==0){
            executando=0;
        }else{
            dprintf(STDOUT_FILENO,"%d %s",recebido,Primo(recebido)?" é primo\n":"não é primo\n" );            
        }

    }

    close(Pipe[0]);                       
    //_exit(0);
    return 0;
}

int main(){

    //fork cria dois processos e cada processo roda uma função

    pid_t pid;

    int totalProdutos=0;
    int descritoresPipe[2];
    //descritoresPipe[0] - leitura do pipe
    //descritoresPipe[1] - escrita no pipe


    printf("Esse programa produz e consome produtos através do pipe!\n");

    printf("Quantos produtos você gostaria de gerar?\n");

    scanf("%d", &totalProdutos);

    //Criando o pipe e colocando em um if para caso gere erro
    if(pipe(descritoresPipe)){
        printf ("Falha na criação do Pipe.\n");
        return ERRO;
    }

    //Para gerar um novo processo é realizado um fork
    //if para detectar falhas

    pid = fork ();
    srand(time(NULL));


    if (pid < 0){
        printf ("Falha no fork.\n");
        return ERRO;
    }

    //Trecho que o filho executa
    else if(pid==0){
        Consumidor(descritoresPipe);
    }
    //Trecho que o pai executa
    else{
        Produtor(descritoresPipe, totalProdutos);

    }

    return 0;

}