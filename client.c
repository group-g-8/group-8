#include<stdio.h>
#include<stdlib.h>

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
int main(){
	int choose;
	char response[1024];
	char message[1024];

	//creating a socket
	
        int net_socket = socket(AF_INET, SOCK_STREAM, 0);
        
        // adress of the socket
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(9000);
        server_address.sin_addr.s_addr = INADDR_ANY;
        
        int conn_status = connect(net_socket,(struct sockaddr*) &server_address, sizeof(server_address));

        if(conn_status == -1){
            printf("\t\tERROR connection to the server failed!!!\n");
        }else{
            printf("\t\tConnection to the server established.....\n");
        }
    while(1){
        //getting client commands
        gets(message);
        if(strcmp(message,"1")==0){
            break;
        }else{
            strcat(message," ");
        }
        printf("\t\t%s\n",message);
        //sending data to the server		
        send(net_socket, message, sizeof(message), 0);

        //recieving from the server
        recv(net_socket, response, sizeof(response), 0);
        printf("\t\t-------------------------------\n");
        printf("\t\t%s\n",message);
        printf("\t\t%s\n",response);
        printf("\t\t-------------------------------\n");
        close(net_socket);
	}

	return 0;
}
