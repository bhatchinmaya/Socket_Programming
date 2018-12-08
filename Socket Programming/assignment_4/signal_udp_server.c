
void signal_handle(int sign);
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

#define LEN 100
int server_socket;
char buffer[LEN];



int main(int argc, char **argv)
{
	
	struct sockaddr_in server_addr,other_addr;
	//create socket
	server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_socket == -1)
	{
		perror("FAIL:server socket");
	}

	printf("created server socket\n");
	//fillin server info
	
	memset(&server_addr,0,sizeof(server_addr));
	memset(&other_addr,0,sizeof(other_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(3200);
	//server_addr.sin_addr.s_addr = htonl(inet_addr("127.0.0.1"));
	server_addr.sin_addr.s_addr = INADDR_ANY;

	//binding
	int server_bind = bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr));

	if(server_bind == -1)
	{
		perror("FAIL:server_bind");
	}

	//set up signal handler
	signal(SIGIO,signal_handle);	
	

	int r_own = fcntl(server_socket,F_SETOWN,getpid());
	if (r_own == -1){
		perror("FCNTL SETOWN failed");
	}
	int r_fl = fcntl(server_socket,F_SETFL,FASYNC);
	if (r_fl == -1){
		perror("FCNTL SETFL failed");
	}
	for(;;)
	;

}


void signal_handle(int sign)
{
	int recvb;
	int sendb;
	int addr_len;
	struct sockaddr_in connectad;

	//printf("signal handler\n");
	addr_len = sizeof(connectad);
	recvb = recvfrom(server_socket,buffer,LEN,0,(struct sockaddr *)&connectad, &addr_len);
	if( recvb == -1){
		perror("FAILED:recvfrom"); 
	}
	buffer[recvb]='\0';
	printf("receive msg: %s \nfrom addr: %s\n",buffer,inet_ntoa(connectad.sin_addr));
	sendb = sendto(server_socket, buffer, sizeof(buffer), 0, (const struct sockaddr *)&connectad, addr_len);
	if(sendb == -1)
	{
		perror("FAIL:server_send");
	}
	printf("send back\n");
	return;
}

