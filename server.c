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

#define PORT 2000
#define BACKLOG 5
#define LENGTH 1024 

#include <mysql/mysql.h>
#include <mysql/my_global.h>

//initializing the mysql database connections
static char*host = "localhost";
static char*user = "root";
static char*password = "ichadsuubi";
static char*dbname = "utf";
unsigned int port = 3306;
char mysqlapi(char a);

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

	/* Get the Socket file descriptor */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
	    error("ERROR: Failed to obtain Socket Descriptor.\n");
	}
	else 
		printf("[Server] Obtaining socket descriptor successfully.\n");

	/* Fill the client socket address struct */
	addr_local.sin_family = AF_INET; // Protocol Family
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
		bzero(revbuf, LENGTH); 

		char actual[LENGTH];
		char cy[LENGTH];

		strncpy(actual, revbuf, LENGTH);
		strncpy(cy, revbuf, LENGTH);

		char content[100];
		char cmd[50];
		strncpy(actual, revbuf, LENGTH);
		sscanf(actual,"%s %s",cmd,content);

		int fr_block_sz = 0;
		
		fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0);

		if (strcmp(revbuf,"check_status")==0)
		{
			check_status(nsockfd);
		}
		
		else
		{
			char command[50];
			char query[50];

			sscanf(revbuf,"%s %s",command,query);
			printf("%s\n",command);
			if (strcmp(command,"get_statement")==0)
			{
				get_statement(nsockfd,query);
			}
			else if (strcmp(command,"check_status")==0)
			{
				check_status(nsockfd, query);
			}
			else if(strcmp(command,"Addmember")==0){
				add_member(nsockfd,query);
			}
			else if (strcmp(command,"search")==0)
			{
				search(nsockfd,query);
			}
			else
			{
				//add_from_file(nsockfd, revbuf);
				add_member(nsockfd,revbuf);
			}	
		}
		
		
		
	}
	// close(nsockfd);
	// printf("[Server] Connection with Client closed. Server will wait now...\n");
}

void add_member(int sock, char mem_file[1024]){
	FILE *f;

	printf("Client message recieved successfully\n");
	f = fopen("storage/app/public/district_files/member.txt", "a"); //opening the file records.txt
	mem_file [ strcspn(mem_file, "\n\r") ] = 0;
	fprintf(f,"%s\n",mem_file);
	fclose(f);

	//sending reply to the client
	char resp[100];
	strcpy(resp, "Member added successfully");
	send(sock, resp, sizeof(resp), 0);
	bzero(resp, 100);
}

void add_from_file(int client, char buf[LENGTH]){
	/*Receive File from Client */
	char* fr_name = "storage/app/public/district_files/member.txt";
	FILE *fr = fopen(fr_name, "a");
	if(fr == NULL)
	{
		printf("File %s Cannot be opened file on server.\n", fr_name);
	}
	else
	{
		int fr_block_sz = 0;
		//int write_sz = fwrite(buf, sizeof(char), fr_block_sz, fr);
		//revbuf [ strcspn(revbuf, "\n\r") ] = 0;

		int write_sz = fprintf(fr, "%s", buf);
		bzero(buf, LENGTH);
		
		printf("Ok received from client!\n");
		fclose(fr);

		//sending reply to the client
		char resp[100];
		strcpy(resp, "Member added successfully from the file");
		send(client, resp, sizeof(resp), 0);
		bzero(resp, 100);
	}
}

void check_status(int sock, char agent[20]){
    FILE *fd;                           // File pointer
    char filename[] = "storage/app/public/district_files/member.txt";       // file to read
    static const long max_len = 55+ 1;  // define the max length of the line to read
    char buff[max_len + 1];             // define the buffer and allocate the length

    if ((fd = fopen(filename, "rb")) != NULL)  {      // open file. I omit error checks

        fseek(fd, -max_len, SEEK_END);            // set pointer to the end of file minus the length you need. Presumably there can be more than one new line caracter
        fread(buff, max_len-1, 1, fd);            // read the contents of the file starting from where fseek() positioned us
        fclose(fd);                               // close the file

        buff[max_len-1] = '\0';                   // close the string
        char *last_newline = strrchr(buff, '\n'); // find last occurrence of newlinw 
        char *last_line = last_newline+1;         // jump to it

        printf("captured: %s\n", last_line);

		char message[50];
		char ag[50];

		sscanf(last_line,"%s %s",message,ag);
		printf("%s\n",ag);
		if (strcmp(message,"invalid")==0)
		{
			if (strcmp(ag,agent)==0)
			{
				char resp[200];
				strcpy(resp, "Your signature is wrong, please correct it");
				send(sock, resp, sizeof(resp), 0);
				bzero(resp, 200);
			}
			else{
				char resp[200];
				strcpy(resp, "The file is valid, proceed with enrollment");
				send(sock, resp, sizeof(resp), 0);
				bzero(resp, 200);
			}
		}else{
			char resp[200];
			strcpy(resp, "The file is valid, proceed with enrollment");
			send(sock, resp, sizeof(resp), 0);
			bzero(resp, 200);
		}
		
		
    }
}

void get_statement(int client,char agent_username[20]){
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL *conn;
    conn= mysql_init(NULL);

    //connection to the mysql database
    if(!(mysql_real_connect(conn,host,user,password,dbname,port,NULL,0))){
        fprintf(stderr,"\n %s [%d]\n",mysql_error(conn),mysql_errno(conn));
        exit(1);
    }
    else
    {
        printf("connection to the database successful\n");		
        char query[]="select concat(amountpaid, ' ', paymentdate) as result from payments,agents where payments.payment_id=agents.agentid and agents.userName='";
		strncat(query,agent_username,20);
		strncat(query,"'",2);

        char statement[1024];
        printf("connected....\n");
        sprintf(statement,query);

        //querying the select statement
        mysql_query(conn,statement);
            
        
        res = mysql_store_result(conn);
        int counter=0;
		char resu[LENGTH];
        //fetching from the database			
        while((row = mysql_fetch_row(res))!=NULL)
        {
            if(counter>1) continue;
            printf("%s\n",row[0]);
            char *hold;
            hold=row[0];// initializing the result from the database to the variable hold
            if(hold==NULL){
                char result[100]="Result not found\n";
                send(client, result, sizeof(result), 0);
                printf("Client reply sent successfully\n");
				bzero(result,100);
            }else{
                //sending the result back to the client
				strcpy(resu,hold);
				printf("Client reply sent successfully: %s\n",resu);
                send(client, resu, sizeof(hold), 0);
                printf("Client reply sent successfully: %s\n",hold);
				bzero(hold, sizeof(hold));
				bzero(resu, sizeof(resu));
                counter++;
            }
        }
        mysql_free_result(res);
        mysql_close(conn);
        
    }
}

void search(int client, char criteria[256]){
    printf("%s",criteria);
    char line[256];
    char newline[256];

	// char command[50];
	// char query[50];

	// sscanf(criteria,"%s %s",command,query);
	FILE *fp;
	fp = fopen("storage/app/public/district_files/member.txt","r");
	if (fp==NULL){
		printf("Error file not found;");
        //break;
	}
	
	criteria = strtok(criteria,",");
	while(!feof(fp)){
		fgets(line,256,fp);
		if(strstr(line,criteria)!= NULL){
			printf("\n%s",line);
			strcpy(newline,line);
			send(client,newline,sizeof(newline),0); 
   		}
 	}
	fclose(fp);
}



void correct_signature(){
	char *name;
    char *date;
	char *gender;
    char *rmd;
	char *sign;
	char *username;
    char *district;
	char line[1024];

	char st[]= "alice_nakibuuka,2019-07-01,f,eric,H,nash,kampala";

	name = strtok(st,",");
	date = strtok(NULL,",");
	gender = strtok(NULL,",");
	rmd = strtok(NULL,",");
	sign = strtok(NULL,",");
	username = strtok(NULL,",");
	district = strtok(NULL,",");

	bzero(line,1024);
	strncat(line,name,20);
	strncat(line,",",2);
	strncat(line,date,20);
	strncat(line,",",2);
	strncat(line,gender,20);
	strncat(line,",",2);
	strncat(line,rmd,20);
	strncat(line,",",2);
	strncat(line,sign,20);
	strncat(line,",",2);
	strncat(line,username,20);
	strncat(line,",",2);
	strncat(line,district,20);
	printf("%s\n",line);
}