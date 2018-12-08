#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char  **argv)
{
 	



	struct sockaddr_in server_addr;
   	server_addr.sin_family = AF_INET;
  	 server_addr.sin_port = htons(3200);
  	 server_addr.sin_addr.s_addr = INADDR_ANY;
      int server_socket = socket(AF_INET, SOCK_STREAM, 0);
   if(server_socket == -1)
   { 
      perror("error in creating server socket");
   }
   
  int optval = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  int server_bind = bind(server_socket,(struct sockaddr*)&server_addr, sizeof(server_addr));
   if(server_bind == -1)
   { 
      perror("error in binding server socket");
   }

	// limit the number of users to 10
	int server_listen = listen(server_socket, 10);
   	if(server_listen == -1)
   	{ 
      	perror("error in listening");
   	}
	
   	//int server_accept = accept(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
while(1){  	
	char recv_buffer[2000];
	int server_accept = accept(server_socket, NULL, NULL);
   	if(server_accept == -1)
   	{ 
      	perror("error in accepting");
   	}
	

	struct sockaddr peer;
	socklen_t peer_len = sizeof(peer);
	if(getpeername(server_accept,&peer,&peer_len)==-1){
		perror("error in get peer name");
	}

	char host[100];
	int hostname = gethostname(host,sizeof(host));
	if (hostname == -1) { 
        	perror("error in gethostname"); 
    	} 
	printf("hostname:%s\n",host);

	char buf[1024];
	int nameinfo = getnameinfo(&peer,peer_len,buf,sizeof(buf),0,0,0);
	if (nameinfo==-1){
		perror("error in get name infor");
	}
	printf("clientname:%s\n",host);



    	int server_recv = recv(server_accept, recv_buffer, 2000,0);
   	if(server_recv == -1)
   	{ 
      	perror("error in reading");
   	}

	char * recv_arr;
	char * file_name;
	char * test,* route_addr,* test1;
	printf("received msg is: %s\n", recv_buffer);
    	//printf("\n\nrecvmsg_here:\n%s\n", recv_buffer);
    	recv_arr = strtok(recv_buffer,"/");
    	//printf("token after split:%s\n",recv_arr);//GET
	
	route_addr = strtok(NULL," HTTP");
	//printf("route address: %s\n",route_addr);
	
	test = strtok(NULL," \n"); //HTTP
	//printf("rest:%s\n",test);
	int num = sizeof(test);	
	char version[num];
	int ii;
	
	for(ii=0;ii<num;ii++){
		version[ii] = *test++;
	}
	char status_code[30];
	strcpy(status_code,"initial");
	if(strncmp(recv_arr,"GET",3)){
		strcpy(status_code,"501 Method Not Implemented\n");
	}


	file_name = route_addr;

	
	//read file
   	FILE *file;
	int len;	
	file = fopen(file_name, "r");
	if(file==NULL){
		strcpy(status_code,"404 Not Found\n");
	}
	fseek(file,0,SEEK_END);
	len = ftell(file);
	fseek(file,0,SEEK_SET);
	//printf("length:%d\n",len);
	char read_buffer[len+1];
  	fread(read_buffer,len,1, file);
  	//printf("data in test file: %s \n", read_buffer);
   	fclose(file);
	

	//content type
	
	char *filetype;

	int i;
	filetype = strtok(file_name,".");
	filetype = strtok(NULL,".");
	

	char * content_type;

	switch(filetype[0])
	{
		case 't':
			content_type = "text/plain\n";
			break;
		case 'p':
			content_type = "image/png\n";
			break;
		case 'h':
			content_type = "text/html\n";
			break;
	}

	char str_len[10];
   	
  	//char send_buffer[2000] = "HTTP/1.1 200 OK\n";
	char send_buffer[1000];
	char zero[] =" ";
	memcpy(send_buffer, zero, sizeof send_buffer);
	strncpy(send_buffer,version,num);
	int length_send;
	
	printf("\nsend buffer content:\n\r%s\n\r",send_buffer);
	strcat(send_buffer,status_code);
	
	//printf("\nsend buffer content2:\n\r%s\n\r",send_buffer);
	sprintf(str_len,"%d",len); //convert int to char
	strcat(send_buffer,"Content-Type:");
	strcat(send_buffer,content_type);
	strcat(send_buffer,"Content-Length:");
	strcat(send_buffer,str_len);
	strcat(send_buffer,"\r\n\r\n");
	printf("send buffer(status):\n\r%s\n\r",send_buffer);
	length_send = strlen(send_buffer);
	printf("return status(bytes):%d\n\r%s\n",length_send,send_buffer);
	int server_send_s = send(server_accept, send_buffer, strlen(send_buffer), 0); 	
	if(server_send_s == -1){
      		perror("error in sending status code");
   	}
	else{
      		int server_send_c = send(server_accept, read_buffer, len, 0);
		printf("send message body(bytes):%d\n",server_send_c);
		if(server_send_c == -1){
      		perror("error in sending status code");
   		}
   	}
	
	printf("return send headers(bytes):%d\n",server_send_s); 
	
   	void *ptr = &recv_buffer;
   	

   	printf("data sent successfully \n\n\n\n");

        }



}

