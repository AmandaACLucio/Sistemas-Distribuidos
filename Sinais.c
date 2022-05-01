#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#define ERRO               1


int enviaSinal(pid_t pidDestino, int signal){

    if (getpgid(pidDestino) >= 0) {
        printf("Processo existe\n");
        kill(pidDestino, signal);
        sleep(2);
        return 0;
    
    }else{
        printf("Processo não existe\n");

        return 1;
    }
}

int recebeSinal(){

    pid_t pid;
    pid = getpid(); //getpid retorna pid do processo chamado
    printf("Pid é %d \n",pid); //exibindo pid

    sigset_t sigset; //máscara (set) do tipo sigset_t que será passada como parâmetro para bloqueadores

    
    int opcaoWhile, sinalEsperado, sinal;
    int *sinalptr = &sinal; //ponteiro utilizado em sigwait para receber o valor do sinal recebido
    opcaoWhile=0;

    //esvaziando máscara
    sigemptyset(&sigset);

    //adicionado na máscara para sigwait bloquear
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGUSR2);
    sigaddset(&sigset, SIGTERM);

    //sigprocmask permite continuar processo msm após receber um dos sinais
    sigprocmask( SIG_BLOCK, &sigset, NULL );

    while(opcaoWhile !=3){

        //variável registra sinal recebido e bloqueado
        sinalEsperado = sigwait(&sigset, sinalptr);

        if(sinalEsperado == -1){
            printf("Erro em sigwait\n");
            return 1;
        }else {
            //SIGUSR1 recebido
            if(*sinalptr == SIGUSR1){
                printf("Sinal SIGUSR1\n");
            
            //SIGUSR2 recebido
            }else if(*sinalptr == SIGUSR2){
                printf("Sinal SIGUSR2\n");
            
             //SIGTERM recebido           
            }else if(*sinalptr == SIGTERM){
                printf("SIGTERM foi recebido, logo o processo irá terminar\n");
                opcaoWhile =3;
                //int matarProcesso;
                //matarProcesso =  kill(pid, 9);
            }
        }
    }

    return 0;
}

int main(){
    
    ///Decidir se recebe sinal ou envia 
    
    /*pid_t pidDestino;
    pidDestino = 101513;
    
    enviaSinal(pidDestino, 15);*/
    recebeSinal();


}