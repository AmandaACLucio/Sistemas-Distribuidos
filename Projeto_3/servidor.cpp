#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <pthread.h>
#include <queue>
#include <mutex>
#include <fstream>

using namespace std;
std::mutex mutex_queue;
void writeFile(string client, string messages){
// Create and open a text file
  ofstream MyFile("log.txt");

  // Write to the file
  MyFile << client + messages;

  // Close the file
  MyFile.close();
}

 void *handle_connection(void* p_listening){
    int listening = *((int*)p_listening);
    // Aguarde uma conexão
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
 
    char host[NI_MAXHOST];      // Nome do cliente remoto
    char service[NI_MAXSERV];   // Serviço (ou seja, porta) em que o cliente está conectado
 
    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
 
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }
 
    // fechar listening socket
    close(listening);
 
    // While loop: aceita e faz eco da mensagem de volta ao cliente
    char buf[4096];
 
    while (true)
    {
        memset(buf, 0, 4096); //limpa o buffer
        // Aguarde o cliente enviar dados
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        
        // printf("%s", buf);

        if (bytesReceived == -1)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }
        cout << string(buf, 0, bytesReceived) << endl;
 
        // Echo Mensagem de eco de volta ao cliente
        send(clientSocket, buf, bytesReceived + 1, 0);
    }
 
    // Fechar o  socket
    close(clientSocket);
    //return ;
}
void *newSocket( void *ptr)
{
// Criar um soquete
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        //return;
    }
 
    // Vincule o endereço IP e a porta a um soquete
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
 
    bind(listening, (sockaddr*)&hint, sizeof(hint));
 
    // Diga ao Winsock que o soquete é para ouvir
    listen(listening, SOMAXCONN);

    pthread_t thread2, thread3;
    //char *message1 = "primeira thread\n";
    int *plistem = (int*) malloc(sizeof(int));
    *plistem = listening;
    int algo = pthread_create( &thread2, NULL, handle_connection,plistem);
    handle_connection(plistem);
}


 
int main()
{
    int n = 2; //processos
    int r = 10; //repetições
    int k = 2; //
    queue<string> mensagens = queue<string>();
    pthread_t thread1,thread2;
    //char *message1 = "primeira thread\n";
    int algo = pthread_create( &thread1, NULL, newSocket,NULL);
    //int algo2 = pthread_create( &thread2, NULL, newSocket,NULL);
    //newSocket();
    int op = 0;
    writeFile("teste ", "| teste realizado");
    while(1){
        mensagens.push("client 1 ");
        mensagens.push("client 8 ");
        mensagens.push("client 3 ");
        mensagens.push("client 2 ");
        printf("** Informe uma opcao: **\n \n");
        printf("1) imprimir a fila de pedidos atual \n");
        printf("2) imprimir quantas vezes cada processo foi atendido \n");
        printf("3) encerrar a execução. \n");
        scanf("%d", &op);

        if (op == 1)
        {
            mutex_queue.lock();
            queue <string> cop = mensagens;
            mutex_queue.unlock();
            while (!cop.empty())
            {
                std::cout << cop.front() << " \n";
                cop.pop();
            }
            /* code */
        }else if (op == 2)
        {

        }else if (op == 3){

        }
    }
    return 0;
}
