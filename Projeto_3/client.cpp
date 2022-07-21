#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h>
#include <sys/wait.h>

#define PORT 54000 
using namespace std;

void writeFile(string client, string messages){
// Create and open a text file
  ofstream MyFile("log.txt");

  // Write to the file
  MyFile << client + messages;

  // Close the file
  MyFile.close();
}

string valueTimeinfo()
{
  time_t tmp = {0};
  struct tm timeinfo = *localtime(&tmp);
  return asctime (timeinfo);
}

int connectSocket()
{
	int sock = 0; 
	struct sockaddr_in serv_addr;
	
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		cout<<<"Error creating socket"<<endl; 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convertion addresses to binary 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		cout<<<"Invalid Address"<<endl; 
		return -1; 
	} 
	
	while((connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)) 
	{ 
		cout<<<"Connection Failed"<<endl; 

	}
	return sock;
}

int request(char* buffer, char* PIDserver, int socket)
{
	int valueRead;

	strncpy(buffer, "1|\0", 1024);
	strcat(buffer,PIDserver);

	dprintf(1,"Enviando mensagem %s\n",buff);

	send(socket , buffer , 1024 , 0 ); 
	valueRead = read(socket , buffer, 1024);

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

void release(char* buffer,char* PIDserver,int socket)
{
	strncpy(buffer, "3|\0",1024);
	strcat(buffer, PIDserver);

	dprintf(1,"Enviando mensagem %s\n",buffer);
    
	send(socket , buffer , 1024 , 0 );
	dprintf(1,"Release Critic Region\n"); 	
}

void createRequests(int countProcess,char* buffer,char* PIDserver,int socket,int pointerBuffer)
{
	int sucess;
	for(int j=0;j<countProcess;j++)	
	{
		sucess = request(buffer,PIDserver,socket);
		if(sucess)
		{
			writeFile(PIDserver, pointerBuffer);
			release(buffer, PIDserver, socket);
		}
	}
	close(socket);
}

int main(int argc, char const *argv[]) 
{ 
    int n = atoi(argv[1])-1; //number of process
	int r = atoi(argv[2]); //number of repetitions
  	int k = atoi(argv[3]); //tempo de espera
  	int *pids = malloc(n*sizeof(int));

	for (int i = 0; i < n; i++)
  	{
    		if ((pids[i] = fork()) < 0)
    		{
    			exit(1);
    		}
    		else 
    		{
    			int process_id = getpid();
                
                char PIDserver[1024];
                char buffer[1024] = {0};
                
                sprintf(PIDserver,"%d|",process_id); 
                
                int socket = connectSocket();
                createRequests(r,buffer,PIDserver,socket,k);
    		}
    	}

    int response;
	int pid;
    
	while (n > 0)
	{
		pid = wait(&response);
		--n;
	}
	return 0; 
} 
