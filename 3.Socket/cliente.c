#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080


int recive(){
    int N = 5;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    //char buffer2[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Converte endereços IPv4 e IPv6 de texto para binário
    // Formato
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
 
    if (connect(sock, (struct sockaddr*)&serv_addr,
                sizeof(serv_addr))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    int Ni = 0;
    char* hello2, text[32] ;
    for(int i = 0; i < N; i++){
        //printf("interacao %d\n", i);
        Ni = (rand() % 100) + Ni;
        sprintf(text, "%d", Ni);
        hello2 = text;
        send(sock, hello2, strlen(hello2), 0);
        //printf("numero aleatorio: %d\n", Ni);
        valread = read(sock, buffer, 1024);
        printf("%s.\n", buffer);
        //printf("fim interacao %d\n", i);
    }
    // send(sock, hello, strlen(hello), 0);
    // int algo = 123456789;
    // char* hello2, text[32] ;
    // printf("%s", hello2);
    // sprintf(text, "%d", algo);
    // hello2 = text;
    // send(sock, hello2, strlen(hello2), 0);
    printf("Hello message sent\n");
    //valread = read(sock, buffer2, 1024);
    //printf("%s.\n", buffer2);
    return 0;
}
int main(int argc, char const* argv[])
{
    recive(argc, *argv);
}