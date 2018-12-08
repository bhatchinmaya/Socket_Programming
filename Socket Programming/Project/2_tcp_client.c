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
//initialization
	fd_set readfds;
	struct sockaddr_in clnt_addr;
	clnt_addr.sin_family = AF_INET; 
    	clnt_addr.sin_addr.s_addr = INADDR_ANY;
    	clnt_addr.sin_port = htons(3200);
	int clnt_addr_len = sizeof(clnt_addr);


	int clnt_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (clnt_socket == -1)
	{
		perror("error in creating client socket");
	}

	int optval = 1;
  	setsockopt(clnt_socket, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int)); 

	int clnt_connect = connect(clnt_socket, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr));
	if (clnt_connect == -1)
	{
		perror("error in connect");
	}
	// to get the ip address and port number
		socklen_t addressLength = sizeof(clnt_addr);
		getsockname(clnt_socket, (struct sockaddr*)&clnt_addr, &addressLength);
		printf("local address: %s\n", inet_ntoa( clnt_addr.sin_addr));
		printf("local port: %d\n", (int) ntohs(clnt_addr.sin_port));

while(1)
  {	


	FD_ZERO(&readfds);
	FD_SET(0, &readfds);	
	FD_SET(clnt_socket, &readfds);
	char buf[BUFSIZE]; // max length of the message
	
	int select_value = select(clnt_socket+1, &readfds, 0, 0, 0);
	if (select_value == -1)
	{
		perror("error in select");
	}

	if (FD_ISSET(0, &readfds))
         {
		memset(buf, 0, BUFSIZE);
        	fgets(buf, BUFSIZE, stdin);
	 	printf("data typed:%s\n", buf);
			if (buf[0] != '\n')
				{
					
				
		int send_clnt = send(clnt_socket, buf, strlen(buf), 0);
		if (send_clnt == -1)
		{
			perror("error in sending");
		}
				}
    	 }
	
	
	if (FD_ISSET(clnt_socket, &readfds))
      	 {
	
		char recv_buffer[1000];
		memset(recv_buffer, 0, sizeof(recv_buffer));
		int recv_clnt = recv(clnt_socket, recv_buffer, 1000, 0); //set the receive buffer size
		if (recv_clnt == -1)
		{
			perror("error in receiving");
		}
		printf("data from server:%s\n", recv_buffer);
	
         } 	
  }


}
