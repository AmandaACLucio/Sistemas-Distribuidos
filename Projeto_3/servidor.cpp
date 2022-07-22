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
#include <bits/stdc++.h>

using namespace std;
std::mutex mutex_queue;
std::mutex mutex_processos;

int n = 100;  //processos
int r = 10; //repetições
int k = 2;  //segundos

void writeFile(string client, string messages){
// Create and open a text file
  ofstream MyFile("log.txt", std::ios::app);

  // Write to the file
  MyFile << client +  messages << "\n";

  // Close the file
  MyFile.close();
}
void readFile(){
    string myText;
    ifstream MyReadFile("log.txt");
    while (getline (MyReadFile, myText)) {
        // Output the text from the file
        cout << myText << endl;
    }
    // Close the file
    MyReadFile.close();
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
        if(string(buf, 0, bytesReceived).find("REQUEST") != -1)
        {
           mutex_queue.lock();
           writeFile(string(buf, 0, bytesReceived),"");
           mutex_queue.unlock();
           char new_buf[1024];
           char new_buf2[1024];
           mutex_processos.lock();
           strncpy(new_buf, "GRANT|\0", 1024);
           writeFile(string(new_buf),std::to_string(bytesReceived));
           cout << (string(new_buf, 0, bytesReceived)) << endl;
           send(clientSocket, new_buf, bytesReceived + 1, 0);
           int bytesReceived2 = recv(clientSocket, new_buf2, 4096, 0);
           cout << "esperando release: " << (string(new_buf2, 0, bytesReceived2)) << endl;
           writeFile(string(new_buf2),std::to_string(bytesReceived2));
           mutex_processos.unlock();
        }             
        // Echo Mensagem de eco de volta ao cliente
        //send(clientSocket, buf, bytesReceived + 1, 0);
    }
 
    // Fechar o  socket
    close(clientSocket);
    return 0;
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
    pthread_t id[4];
    //char *message1 = "primeira thread\n";
    int *plistem = (int*) malloc(sizeof(int));
    *plistem = listening;
    for (size_t i = 0; i < n; i++)
    {
        int algo = pthread_create( &id[i], NULL, handle_connection,plistem);
    }
    
    //int algo = pthread_create( &thread2, NULL, handle_connection,plistem);
    handle_connection(plistem);
    return 0;
}
 
int main()
{
    pthread_t thread1,thread2;
    //char *message1 = "primeira thread\n";
    int algo = pthread_create( &thread1, NULL, newSocket,NULL);
    //int algo2 = pthread_create( &thread2, NULL, newSocket,NULL);
    //newSocket();
    int op = 0;
    while(1){
        printf("** Informe uma opcao: **\n \n");
        printf("1) imprimir a fila de pedidos atual \n");
        printf("2) imprimir quantas vezes cada processo foi atendido \n");
        printf("3) encerrar a execução. \n");
        scanf("%d", &op);

        if (op == 1)
        {
            mutex_queue.lock();
            readFile();
            mutex_queue.unlock();
        }else if (op == 2)
        {

        }else if (op == 3){
            exit(1);
        }
        else{
            std::cout << "Informe uma entrada valida" << " \n";
        }
    }
    return 0;
}
