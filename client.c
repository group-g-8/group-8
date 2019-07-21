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
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 9000
#define LENGTH 512 


void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	/* Variable Definition */
	int sockfd; 
	int nsockfd;
	char revbuf[LENGTH];
    char command[LENGTH];
    char response[LENGTH];

	struct sockaddr_in remote_addr;

	/* Get the Socket file descriptor */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
	    error("ERROR: Failed to obtain Socket Descriptor!\n");
	}

	/* Fill the socket address struct */
	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(PORT); 
	inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr); 
	bzero(&(remote_addr.sin_zero), 8);

	/* Try to connect the remote */
	if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
	{
	    error("ERROR: Failed to connect to the host!\n");
	}
	else {
		printf("[Client] Connected to server at port %d...ok!\n", PORT);
    }

    printf("Welcome, please enter your username\n");
    printf("Username:   ");
    gets(command);
    printf("\n");

	commands();
    while (1)
    {
        
        printf("client:  ");
        gets(command);
        printf("\n");
        
        if (strcmp(command,"1")==0)
        {
            break;
        }else
        {
            if (strcmp(command,"Addmember utf.txt")==0)
            {
                char* fs_name = "utf.txt";
                char sdbuf[LENGTH]; 
                printf("client: Sending %s to the Server...\n", fs_name);
                FILE *fs = fopen(fs_name, "r");
                if(fs == NULL)
                {
                    printf("ERROR: File %s not found.\n", fs_name);
                    exit(1);
                }

                bzero(sdbuf, LENGTH); 
                int fs_block_sz; 
                //int success = 0;
                while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs))>0)
                {
                    if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
                    {
                        printf("ERROR: Failed to send file %s.\n", fs_name);
                        break;
                    }
                    bzero(sdbuf, LENGTH);
                }
                printf("Success:    File %s from Client was Sent!\n", fs_name);
            }else
            {
                //sending data to the server		
                send(sockfd, command, sizeof(command), 0);

                //recieving from the server
                recv(sockfd, response, sizeof(response), 0);
                printf("server:  %s\n",response);
            }
            
        }   
    }
    
    

	close (sockfd);
	printf("[Client] Connection lost.\n");
	return (0);
}


void commands(){
    printf("Please choose from the following commands\n");
    printf("-----------------------------------------------------\n");
    printf("1.  Addmember, member_name, date, gender, recommender\n");
    printf("2.  check_status\n");
    printf("3.  get_statement\n");
    printf("4.  Addmember uft.txt\n");
    printf("5.  search, criteria\n");
    printf("-----------------------------------------------------\n");
    
}
