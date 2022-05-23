#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h> 
#include <stdbool.h>
#include <unistd.h>


sem_t semMutex;
sem_t semFull;
sem_t semEmpty;
sem_t semMain;

int *vectorN; //vector shared memory
int sizeN;
int objectiveProduction=100000;
int produced; 
int consumed;
int continueProduction=1;
int continueConsume=1; 

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
    return rand()%10000001;
}

int findPositionFree(int *vectorShared){

    for(int i = 0; i < sizeN; i++){
        if(vectorShared[i] ==0){
            return i;
        }
    }

    return -1; //não tem posição vazia
}

int findPositionBusy(int *vectorShared){

    for(int i = 0; i < sizeN; i++){
        if(vectorShared[i] !=0){
            return i;
        }
    }

    return -1; //não tem posição vazia
}

void Produce(){

    int number_rand = Delta();
    int position_free = findPositionFree(vectorN);

    if(position_free!=-1){

        vectorN[position_free]=number_rand;
    }

}

void Consume(){

    int position_busy = findPositionBusy(vectorN);

    if(position_busy!=-1){

        int number_rand=vectorN[position_busy];
        //dprintf(STDOUT_FILENO,"%d %s",number_rand,Primo(number_rand)?" é primo\n":"não é primo\n" ); 
        
        vectorN[position_busy]=0;//liberando a posição           

    }

}

void *Produtor(void *arg){

    while(continueProduction){

        sem_wait(&semEmpty);
        sem_wait(&semMutex);

        if(produced<objectiveProduction){

            Produce();
            produced++;
        }else{
            continueProduction=0;
        }

        sem_post(&semMutex);
        sem_post(&semFull);
    }


}

void *Consumidor(void *arg){

    while(continueConsume){

        sem_wait(&semFull);
        sem_wait(&semMutex);


        if(consumed<objectiveProduction){

            Consume();
            consumed++;
        }else{
            continueConsume=0;
            sem_post(&semMain);
        }

        sem_post(&semMutex);
        sem_post(&semEmpty);

    }
}

void UnloadMemory(){
    for(int i = 0; i < sizeN; i++){
        vectorN[i]=0;
    }
}

int main(int argc, char* argv[]) {

    srand(time(NULL));
    int totalThreads;

    //console
    /*
    printf("Esse programa consome e produz números de uma memória compartilhada!\n");

    printf("Digite o valor para Np (número de threads do produtor):\n");
    scanf("%d", &Np);

    printf("Digite o valor para Nc (número de threads do consumidor):\n");
    scanf("%d", &Nc);

    printf("Digite o valor para o tamanho de N:\n");
    scanf("%d", &sizeN);
    */


	sizeN = atoi(argv[1]); // tamanho de N
	int Nc = atoi(argv[2]);  // número de threads do consumidor
	int Np = atoi(argv[3]);  // número de threads do produtor
	
    pthread_t threadWork;
	
    sem_init(&semEmpty,0,sizeN);
	sem_init(&semFull,0,0);
	sem_init(&semMutex,0,1);
	sem_init(&semMain,0,0);

	// Medida de tempo
	clock_t timeExecution; 
	timeExecution = clock();  

    //vetor de threads
    totalThreads = Np+Nc;
    pthread_t list_threads[totalThreads];

    vectorN =  (int*) malloc(sizeN*sizeof(int));
    UnloadMemory();

    //criando threads
    int posNc = 0;
    
	for(posNc =0 ; posNc < Nc;posNc++){
		pthread_create(&threadWork,NULL,Consumidor,NULL);
		list_threads[posNc] = threadWork;
	}

	for(int posNp=posNc; posNp < Np;posNp++){
		pthread_create(&threadWork,NULL,Produtor,NULL);
		list_threads[posNp] = threadWork;
	}

	sem_wait(&semMain);//bloquea a main enquanto o p/c ainda atua

	timeExecution = clock() - timeExecution;
    double timeExecutionSeconds = ((double) timeExecution)/CLOCKS_PER_SEC;

    //create csv

    FILE *fpt;
    fpt = fopen("Results.csv", "a+");

    fprintf(fpt, "%lf, %d, %d, %d\n", timeExecutionSeconds, Np, Nc, sizeN);

    fclose(fpt);

	return 0;

	return 0;
}

