#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

int main(int argc, char **argv) 
{
	
//initialize
	int user_counter = 0;
	char *user[10];
	pid_t pid;
	fd_set readfds;
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(3200);
	int serv_addr_len = sizeof(serv_addr);

//create socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		perror("error in creating server socket");
	}
 
	int optval = 1;
  	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int)); 
	     
	int server_bind = bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (server_bind == -1)
	{
		perror("error in binding");
	}

	int server_listen = listen(server_socket, 10);// change the number of clients here
	if (server_listen == -1)  
   	 { 
      		perror("error in listening"); 
    	 } 

// to get the ip address and port number

	struct sockaddr_in localAddress;
	socklen_t addressLength = sizeof(localAddress);
	getsockname(server_socket, (struct sockaddr*)&serv_addr, &addressLength);
	printf("local address: %s\n", inet_ntoa( serv_addr.sin_addr));
	printf("local port: %d\n", (int) ntohs(serv_addr.sin_port));





// initialize to store the received address
	struct sockaddr_in storeAddress[10];
	socklen_t len_addr = sizeof(storeAddress);


while(1) 
{	
	
	FD_ZERO(&readfds);
	FD_SET(server_socket, &readfds);
	char buf[BUFSIZE]; // max length of the message
	
	int select_value = select(server_socket+1, &readfds, 0, 0, 0);
	if (select_value == -1)
	{
		perror("error in select");
	}

	
	if (FD_ISSET(server_socket, &readfds))
        {
	    int server_accept = accept(server_socket, (struct sockaddr *)&serv_addr, &serv_addr_len);
	    if (server_accept == -1)
	    {
		perror("error in accepting");
	    }
//	    printf("value of server_accept: %d\n", server_accept);
	    user_counter = server_accept; // used for store username in 'user' array

	    if ((pid = fork())==0)
	    {
// store the address of clients


	    getpeername(server_accept, (struct sockaddr *)&localAddress, &addressLength);

	    printf("local address: %s\n", inet_ntoa( localAddress.sin_addr));
	   	printf("local port: %d\n", (int) ntohs(localAddress.sin_port));

	   	storeAddress[user_counter].sin_addr = localAddress.sin_addr;
	   	storeAddress[user_counter].sin_port = localAddress.sin_port;
	   	memset(&localAddress, 0, sizeof(localAddress));




			while(1) //continuosly monitor the connections
   				{

				   char recv_username[1000];
					memset(recv_username, 0, 1000);
					if (user_counter == server_accept)
					{
						int recv_user = recvfrom(server_accept, recv_username, 1000, 0, (struct sockaddr *)&serv_addr, &serv_addr_len );
						if (recv_user == -1)
						{
							printf("error in receiving username");
						}

						if(recv_user != 0) //to avoid infinite loop when the clients disconnect
						   {
						user[user_counter] = recv_username;
						printf("username:%s\n", user[user_counter]);
						user_counter++;
						   }

					}



					char recv_buffer[1000];
					memset(recv_buffer, 0, sizeof(recv_buffer));
					int recv_server = recvfrom(server_accept, recv_buffer, 1000, 0, (struct sockaddr *)&serv_addr, &serv_addr_len );
					if (recv_server == -1)
					{
						perror("error in receiving");
					}
					if(recv_server != 0) //to avoid infinite loop when the clients disconnect
					   {
					      printf("data from client: %s\n", recv_buffer);

					      char send_buffer[1000];
					      strncpy(send_buffer, recv_buffer, 1000);
					      int send_server = sendto(server_accept, send_buffer, 1000, 0, (struct sockaddr *)&serv_addr, serv_addr_len);
					      if (send_server == -1)
					      {
					    	  perror("error in sending");
					      }
					   }
				}
	
  	
 

	    }
                }


	}



}



















