#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#define ERRO               1

int main(){
    pid_t pid;
    pid = getpid(); //getpid retorna pid do processo chamado
    printf("Pid é %d \n",pid); //exibindo pid

    while(1){

    }
}