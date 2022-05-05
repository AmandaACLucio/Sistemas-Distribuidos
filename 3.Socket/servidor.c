// Implemente um programa Produtor-Consumidor com resposta com dois processos e sockets para
// fazer a comunica¸c˜ao. Utilize sockets to tipo TCP. Dessa forma, o programa consumidor deve
// fazer o papel de servidor (aguarda conex˜ao) e o programa produtor deve fazer o papel de cliente
// (inicia a conex˜ao).
// Novamente, o programa produtor deve gerar n´umeros inteiros aleat´orios e crescentes (conforme acima) e o programa consumidor deve receber o n´umero e verificar se o mesmo ´e primo.
// Diferentemente do exerc´ıcio acima, o programa consumidor deve enviar uma mensagem ao produtor informando se o n´umero recebido ´e ou n˜ao primo, e este ´ultimo deve imprimir o resultado
// no terminal. Implemente um protocolo bem simples, onde o produtor gera um n´umero, envia
// ao consumidor, e aguarda a resposta de forma bloqueante. O programa produtor deve se conectar ao programa consumidor, enviar o n´umero gerado, e aguardar o resultado. O programa
// consumidor aguarda a conex˜ao, recebe o n´umero gerado, determina se o mesmo ´e primo, envia
// o resultado ao produtor, e volta a esperar a chegada do pr´oximo n´umero.
// Por fim, os programas devem terminar como no exerc´ıcio acima (enviando zero para terminar
// o consumidor, tendo como parˆametro o n´umero de n´umeros a serem gerados).
// Novamente, cuidado com a representa¸c˜ao num´erica! Dica: converta o n´umero para uma
// string de tamanho fixo, ex. 20 bytes, e escreva e leia este mesma quantidade de bytes.
// Teste o seu programa mostrando seu funcionamento para alguns casos. Teste seu programa

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    char* hello = "Hello from server";
    char* mensagem;
 
    // Criando descritor de arquivo de soquete
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
        == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Conectando com força o soquete à porta 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Conectando com força o soquete à porta 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        if ((new_socket
            = accept(server_fd, (struct sockaddr*)&address,
                    (socklen_t*)&addrlen))
            < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
    while(1){
        char buffer[1024] = { 0 };
        valread = read(new_socket, buffer, 1024);
        int numero = atoi(buffer);
        int i = 1;
        if(numero <= 1){
            printf("%c e primo\n", numero);
        }else{
            for(int d = 2; d < numero; d++){
                if (numero%d == 0){
                    printf("%d e primo\n", numero);
                    mensagem = "o numero e primo";
                    i = 0;
                    break;
                }
            }
            if(i){
                printf("%d nao e primo\n",numero);  
                mensagem = "o numero nao e primo";
            }
        }
        send(new_socket, mensagem, strlen(mensagem), 0);
        printf("fIM SERVIDOR\n");
    }
    return 0;
}