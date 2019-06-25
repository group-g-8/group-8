#include<stdio.h>
#include<stdlib.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

int main(){
	//creating a socket
	int serv_socket = socket(AF_INET, SOCK_STREAM, 0);
	char sub[100]="Command sent successfully\n";
	
	// adress of the socket
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9000);
	server_address.sin_addr.s_addr = INADDR_ANY;

	//binding socket to the address and port number
	bind(serv_socket, (struct sockaddr *) &server_address, sizeof(server_address));

	//listening to the clients
	int listn;
	
	listn = listen(serv_socket, 10);

	if(listn==-1){
		printf("\nError while listening to clients.......\n");
	}else{
		printf("listening to clients.......\n");
	}
	
	int client_socket;
	char sent[300];
	char reply[300];
	FILE *records;
	while(1){
	
        //accepting client sockets
        client_socket = accept(serv_socket,NULL, NULL);
        
        if(client_socket==-1){
            printf("Error while accepting client requests......\n");
        }
        else{
            printf("Accepting client requests.......\n");
        }

		int n = recv(client_socket, sent, sizeof(sent), 0);
        if(n==-1){
            printf("Error while recieving .......");
        }else{
            printf("%s\n",sent);
		}
		gets(reply);          
        //sending the result back to the client
        send(client_socket, reply, sizeof(reply), 0);
                    
	    close(client_socket);
        
    }
	close(serv_socket);
	return 0;
}

