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
#include <netdb.h> 
#include <arpa/nameser.h>
#include <netinet/in.h>
#include <sys/unistd.h>

#define BUF_SIZE 500


int main(int argc, char  **argv)
{
   	struct addrinfo hints;
        struct sockaddr_in sock4; 
        struct sockaddr_in6 sock6;
        struct addrinfo *result, *rp;
        int server_socket, status;
//       	struct sockaddr_storage peer_addr;
        struct sockaddr_in peer_addr; 
        socklen_t peer_addr_len;
        ssize_t nread;
        char buf[BUF_SIZE];

       


/*
        if (argc != 2) {
        	fprintf(stderr, "Usage: %s port\n", argv[0]);
               	exit(EXIT_FAILURE);
       	}
*/

       	memset(&hints, 0, sizeof(struct addrinfo));
  //     	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        
        hints.ai_family = AF_UNSPEC;
//        hints.ai_family = AF_INET6;
        
       	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
//       	hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
        hints.ai_flags = AI_NUMERICHOST;
       	hints.ai_protocol = 0;          /* Any protocol */
       	hints.ai_canonname = NULL;
       	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	
	status = getaddrinfo(argv[1],"3200",&hints, &result);
//        status = getaddrinfo(NULL,"3200",&hints, &result);
        if (status != 0){
        	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        	exit(EXIT_FAILURE);
       	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
        	server_socket = socket(rp->ai_family, rp->ai_socktype,
                       rp->ai_protocol);
           printf("ai_family is %d\n", rp->ai_family);
               	if (server_socket == -1)
                   continue;
               	if (bind(server_socket, rp->ai_addr, rp->ai_addrlen) == 0)
                   break;                  /* Success */

               	//close(server_socket);
        }

           if (rp == NULL) {               /* No address succeeded */
               fprintf(stderr, "Could not bind\n");
               exit(EXIT_FAILURE);
           }

           freeaddrinfo(result);           /* No longer needed */
	printf("Success binding\n");
         
    
       
       



   	int server_listen = listen(server_socket, 3);
   	if(server_listen == -1)
   	{ 
      	perror("error in listening");
   	}
	
   	//int server_accept = accept(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

// code for iterative server using for loop
 

       for (int i=0;i<5;i++){

        char recv_buffer[2000];
        printf("Iterations: %d\n", i+1);
  	int server_accept = accept(server_socket, NULL, NULL);
   	if(server_accept == -1)
   	{ 
      	perror("error in accepting");
   	}


 /* 
       char hostname[200];       
       gethostname(hostname, 200);
       printf("Hostname is %s\n", hostname);
      
   */    
       



      
        
        




/*

    	int server_recv = recv(server_accept, recv_buffer, 2000,0);
   	if(server_recv == -1)
   	{ 
      	perror("error in reading");
   	}
*/

        peer_addr_len = sizeof(struct sockaddr_storage);
    //    int server_recv = recvfrom(server_accept, recv_buffer, 2000, 0,(struct sockaddr *) &peer_addr, &peer_addr_len);
        int server_recv = recv(server_accept, recv_buffer, 2000, 0);
        if(server_recv == -1)
   	{ 
      	perror("error in reading");
   	}
       
/*

    struct sockaddr_in addr;
       int length = sizeof(addr);
       int peername;
       
       peername = getpeername(server_socket, rp->ai_addr, &length);
       if (peername == -1){
       perror("error in getpeername");
       }
      printf("Peer IP address: %s\n", inet_ntoa(addr.sin_addr));
      printf("Peer port      : %d\n", ntohs(addr.sin_port));


*/

/*
       char host[1000];
       char service[1000];
       int getname = getnameinfo((struct sockaddr *)&hints.ai_addr,1000, host, 1000, NULL, 0, 1);
       if (getname == 0)
            printf("Received from %s:%s\n",
                     host, service);
        else
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(getname));

*/

	char * recv_arr;
	char * file_name;
	char * test,* route_addr,* test1;
	
    	printf("\n\nrecvmsg_here:\n%s\n", recv_buffer);
    	recv_arr = strtok(recv_buffer,"/");
    	printf("token after split:%s\n",recv_arr);//GET
	
	route_addr = strtok(NULL," HTTP");
	printf("route address: %s\n",route_addr);
	
	test = strtok(NULL," \n"); //HTTP
	printf("rest:%s\n",test);
	char status_code[30];
	strcpy(status_code,"initial");
	if(strcmp(recv_arr,"GET ")){
		strcpy(status_code,"501 Method Not Implemented\n");
	}
/*
	int j;
	j = strcmp(test,"HTTP/1.1");
	printf("compare result: %d\n",j);
	if(strcmp(test,"HTTP/1.1\n")){
		strcpy(status_code,"400 Bad Request\n");
		printf("%s\n",status_code);
		
	}
*/
	//test1 = strtok(route_f,"/");
	//test1 = strtok(NULL," ");

	file_name = route_addr;
	
/*
	printf("file %s\n",file_name);
	test1 = strtok(file_name,"/");

	while(test1 != NULL){
		file_name = test1;		
		test1 = strtok(NULL," ");
		
	}
	printf("filename %s\n",file_name);
*/	

	
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
	printf("length:%d\n",len);
	char read_buffer[len+1];
  	fread(read_buffer,len,1, file);
  	printf("data in test file: %s \n", read_buffer);
   	fclose(file);
	

	//content type
	
	char *filetype;

	int i;
	filetype = strtok(file_name,".");
	filetype = strtok(NULL,".");
	
	//printf("filetype:%s\n",filetype);
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
	//printf("%s\n\n\n\n\n",content_type);
 
	
	if(!strcmp(status_code,"initial")){
		strcpy(status_code," 200 OK\n");}
	printf("status%s\n",status_code);

	char str_len[10];
   	
   	char *store;
  	//char send_buffer[2000] = "HTTP/1.1 200 OK\n";
	char send_buffer[2000];
	
	strcpy(send_buffer,test);
	strcat(send_buffer,status_code);
	sprintf(str_len,"%d",len); //convert int to char
	strcat(send_buffer,"Content-Type:");
	strcat(send_buffer,content_type);;
	strcat(send_buffer,"Content-Length:");
	strcat(send_buffer,str_len);
	strcat(send_buffer,"\n\n");
	//printf("%s\n",send_buffer);
	//printf("send buffer1:\n%s\n",send_buffer);
	strcat(send_buffer,read_buffer);
	//printf("send buffer cat:\n%s\n",send_buffer);
   	//printf("data after concatination %s \n", send_buffer);
   	//char send_buffer[2000] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\n%";
   	//char send_buffer[25] = "hello";
   	void *ptr = &recv_buffer;
   	//int server_send = sendto(server_accept, send_buffer, 3000, 0, (struct sockaddr *) &peer_addr, peer_addr_len);
        int server_send = send(server_accept, send_buffer, 3000, 0);
   	if(server_send == -1)
   	{ 
      	perror("error in sending");
   	}

	
   	printf("data sent successfully \n");
        
  }
        
        

}

