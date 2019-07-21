#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>firstfirst
#include <netdb.h>

#define PORT 9000 
#define BACKLOG 5
#define LENGTH 1024 

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main ()
{
	/* Defining Variables */
	int sockfd; 
	int nsockfd; 
	int num;
	int sin_size; 
	struct sockaddr_in addr_local; /* client addr */
	struct sockaddr_in addr_remote; /* server addr */
	char revbuf[LENGTH]; // Receiver buffer
	char response[LENGTH];
	char sent[LENGTH];

	/* Get the Socket file descriptor */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
	    error("ERROR: Failed to obtain Socket Descriptor.\n");
	}
	else 
		printf("[Server] Obtaining socket descriptor successfully.\n");

	/* Fill the client socket address struct */
	addr_local.sin_family = AF_INET; // Protocol Familyfirst
	addr_local.sin_port = htons(PORT); // Port number
	addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
	bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

	/* Bind a special Port */
	if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
	{
	    error("ERROR: Failed to bind Port.\n");
	}
	else 
		printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n",PORT);

	/* Listen remote connect/calling */
	if(listen(sockfd,BACKLOG) == -1)
	{
	    error("ERROR: Failed to listen Port.\n");
	}
	else
		printf ("[Server] Listening the port %d successfully.\n", PORT);

	int success = 0;
	while(1)
	{
	    sin_size = sizeof(struct sockaddr_in);

	    /* Wait a connection, and obtain a new socket file despriptor for single connection */
	    if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
		{
	        error("ERROR: Obtaining new Socket Despcritor.\n");
		}
	    else 
			printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));
		if(recv(nsockfd, sent, sizeof(sent), 0)==-1){
			printf("Error while recieving .......");
		}
		else
		{
			printf("%s\n",sent);
			char *command;
			char *cm;
			char actual[LENGTH];
			strncpy(actual, sent, LENGTH);

			command = strtok(sent,",");

			if(strcmp(command,"Addmember")==0){
				add_member(nsockfd,actual);
			}
			else
			{
				/*Receive File from Client */
				char* fr_name = "member.txt";
				FILE *fr = fopen(fr_name, "a");
				if(fr == NULL)
					printf("File %s Cannot be opened file on server.\n", fr_name);
				else
				{
					bzero(revbuf, LENGTH); 
					int fr_block_sz = 0;
					
					while(fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) //could it be sockfd?
					{
						if(fr_block_sz < 0)
						{
							error("Error receiving file from client to server.\n");
						}
						int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
						if(write_sz < fr_block_sz)
						{
							error("File write failed on server.\n");
						}
						else if(fr_block_sz)
						{
							break;
						}
						bzero(revbuf, LENGTH);
					}
					printf("File received from client!\n");

					char resp[100] = "File received successfully";
					client_reply(nsockfd,resp);
					fclose(fr);
				}
			
			}

		
		}

		/* Call the Script */
		//system("cd ; chmod +x script.sh ; ./script.sh");

		close(nsockfd);
		printf("[Server] Connection with Client closed. Server will wait now...\n");
	}
}

void client_reply(char response[1024],int sock){
	//sending the result back to the client
    send(sock, response, sizeof(response), 0);
}

void add_member(int sock, char mem_file[1024]){
	FILE *records;
	//writing to the file 
	printf("Client message recieved successfully\n");
	records = fopen("member.txt", "a"); //opening the file records.txt
	fprintf(records,"%s\n",mem_file);
	fclose(records);

	//sending reply to the client
	char res[100] = "Member added successfully";
	client_reply(sock,res);
}
