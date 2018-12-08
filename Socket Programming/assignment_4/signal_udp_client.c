

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>
int main(int argc, char  **argv)
{

	char send_buffer[25];
	char recv_buffer[25];

	
	int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if(client_socket == -1)
	{
		perror("FAIL:client socket");
	}

	struct sockaddr_in clnt_addr;
	memset(&clnt_addr,0,sizeof(clnt_addr));
	clnt_addr.sin_family = AF_INET;
	clnt_addr.sin_port = htons(3200);
	clnt_addr.sin_addr.s_addr = INADDR_ANY;

	while(1){
	printf("Enter the data by client\n");
	scanf("%s",send_buffer);
	printf("data in send buffer %s \n", send_buffer);
	int l_clnt = sizeof(clnt_addr);
	int client_send = sendto(client_socket, send_buffer, sizeof(send_buffer), MSG_CONFIRM, (const struct sockaddr *) &clnt_addr, l_clnt);

       if(client_send == -1)
		{
			perror("FAIL: client_send");
		}

	printf("client send: %s\n",send_buffer);

	int client_recv = recvfrom(client_socket, recv_buffer, sizeof(recv_buffer), MSG_WAITALL,(struct sockaddr *)&clnt_addr, &l_clnt);
	
	if(client_recv == -1)
		{
			perror("client_recv");
		}
    	printf("client recv: %s\n",recv_buffer);

	}

}
