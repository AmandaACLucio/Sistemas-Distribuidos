#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>

#define PORT 54000 
using namespace std;

void writeFile(string client, string messages){
	// Create and open a text file
	ofstream MyFile("resultado.txt", std::ios::app);
	const auto p1 = std::chrono::system_clock::now();

	// Write to the file
	MyFile << client + "|"  << std::chrono::duration_cast<std::chrono::seconds>(
                   p1.time_since_epoch()).count() << "\n";

	// Close the file
	MyFile.close();
	
}

string valueTimeinfo()
{
    time_t tmp;
	time ( &tmp );
	struct tm * timeinfo = localtime ( &tmp );
	return asctime (timeinfo);
}

int connectSocket()
{
	int sock = 0; 
	struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		cout<<"Error creating socket"<<endl; 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convertion addresses to binary 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		cout<<"Invalid Address"<<endl; 
		return -1; 
	} 
	
	while((connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)) 
	{ 
		cout<<"Connection Failed"<<endl; 
		cout<<sock<<"  "<<(struct sockaddr *)&serv_addr<<endl;

		int a;
		cin>>a;

	}

	cout<<"sock"<<sock<<endl;
	return sock;
}

int request(char* buffer, char* PIDserver, int sock)
{
	int valueRead;
// char buf[4096];
	strncpy(buffer, "REQUEST|\0", 1024);
	strcat(buffer,PIDserver);

	dprintf(1,"Enviando mensagem %s\n",buffer);

	send(sock , buffer , 1024 , 0 ); 
	valueRead = read(sock , buffer, 1024);
	//printf("----------------");
	cout << "Resposta: " << string(buffer, 0, valueRead) << endl;
	if(string(buffer, 0, valueRead).find("GRANT") != -1)
	{
		writeFile(std::to_string(sock), "");
		sleep(1);
		strncpy(buffer, "RELEASE|\0", 1024);
		send(sock , buffer , 1024 , 0 ); 
	}
	
	if(valueRead == 0 )
	{	
		dprintf(1,"Problem in value read\n");
	}

	if (strcmp(buffer,"2") == 0)
	{
		dprintf(1,"Start Grant\n");
		return 1;		
	}
	return 0;
}

void release(char* buffer,char* PIDserver,int sock)
{
	strncpy(buffer, "3|\0",1024);
	strcat(buffer, PIDserver);

	dprintf(1,"Enviando mensagem %s\n",buffer);
    
	send(sock , buffer , 1024 , 0 );
	dprintf(1,"Release Critic Region\n"); 	
}

void createRequests(int countProcess,char* buffer,char* PIDserver,int sock,int pointerBuffer)
{
	int sucess;
	for(int j=0;j<countProcess;j++)	
	{
		sucess = request(buffer,PIDserver,sock);
		if(sucess)
		{
			writeFile(PIDserver, to_string(pointerBuffer));
			release(buffer, PIDserver, sock);
		}
	}
	close(sock);
}

int main(int argc, char const *argv[]) 
{ 
	if(argc!=4){
		cout<<"Insira os parâmetros necessários:"<<endl;
		cout<<"./client n r k"<<endl;
		return 0;
	}

    int n = atoi(argv[1])-1; //number of process
	int r = atoi(argv[2]); //number of repetitions
  	int k = atoi(argv[3]); //tempo de espera

	vector<int> pids;
  	//int *pids = malloc(n*sizeof(int));

	for (int i = 0; i < n; i++)
  	{
		try{
			pids.push_back(fork());
		}catch(int e){
			exit(1);
		}

		int process_id = getpid();
		
		char PIDserver[1024];
		char buffer[1024] = {0};
		
		sprintf(PIDserver,"%d|",process_id); 
		
		int socket = connectSocket();
		cout<<"Conexão bem sucedida"<<endl;
		createRequests(r,buffer,PIDserver,socket,k);

	}

    int response;
	//int pid;
    
	while (n > 0)
	{
		wait(&response);
		--n;
	}
	return 0; 
} 
