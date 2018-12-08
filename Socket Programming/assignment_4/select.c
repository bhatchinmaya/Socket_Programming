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




  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
   if(server_socket == -1)
   { 
      perror("error in creating server socket");
   }
   
   printf("server socket created successfully \r\n");
  int optval = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));
   char recv_buffer[2000];
   char send_buffer[2000] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 20\n\nHello world!\n";


   struct sockaddr_in server_addr, clnt_addr;
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(3200);
   server_addr.sin_addr.s_addr = INADDR_ANY;


   int server_bind = bind(server_socket,(struct sockaddr*)&server_addr, sizeof(server_addr));
   if(server_bind == -1)
   { 
      perror("error in binding server socket");
   }

   int server_listen = listen(server_socket, 10);
   if(server_listen == -1)
   { 
      perror("error in listening");
   }




fd_set readfds;
int BUFSIZE = 1000;
char buf[BUFSIZE];
int notdone = 1;
int connectcnt = 0;
int indicator = 0;
while(notdone){



	FD_ZERO(&readfds);          /* initialize the fd set */
    	FD_SET(server_socket, &readfds); /* add socket fd */
    	FD_SET(0, &readfds);        /* add stdin fd (0) */
   	 if (select(server_socket+1, &readfds, 0, 0, 0) < 0) {
   	   error("ERROR in select");
   		 }



	if (FD_ISSET(0, &readfds)) {
      fgets(buf, BUFSIZE, stdin);
      switch (buf[0]) {
      case 'c': /* print the connection cnt */
	printf("Received %d connection requests so far.\n", connectcnt);
	indicator = 20;
	printf("server> ");
	fflush(stdout);
	break;
      case 'q': /* terminate the server */
	notdone = 0;
	break;
      default: /* bad input */
	printf("ERROR: unknown command\n");
	printf("server> ");
	fflush(stdout);
      }
    }  


	if (FD_ISSET(server_socket, &readfds) && (indicator != 20)) {



 
   //int server_accept = accept(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
  int server_accept = accept(server_socket, NULL, NULL);
   printf("server accept %d\n", server_accept);
   if(server_accept == -1)
   { 
      perror("error in accepting");
   }
    connectcnt++;
   int server_read = read(server_accept, recv_buffer, 2000);
   if(server_read == -1)
   { 
      perror("error in reading");
   }
 
   printf("data form client: %s \n", recv_buffer);

   int server_send = send(server_accept, send_buffer, 2000, 0);
   
   
   if(server_send == -1)
   { 
      perror("error in sending");
   }


  

   printf("data sent successfully \n");
         close(server_accept);

	



  }
       else

{
	
 int server_accept = 0;
 //int server_accept = accept(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
 // int server_accept = accept(server_socket, NULL, NULL);
 //  printf("server accept %d\n", server_accept);
 //  if(server_accept == -1)
 //  { 
 //     perror("error in accepting");
 //  }
 //   connectcnt++;
   int server_read = read(server_accept, recv_buffer, 2000);
   if(server_read == -1)
   { 
      perror("error in reading");
   }
 
   printf("data form client: %s \n", recv_buffer);

   int server_send = send(server_accept, send_buffer, 2000, 0);
   
   
   if(server_send == -1)
   { 
      perror("error in sending");
   }


  

   printf("data sent successfully \n");
         close(server_accept);

  



}





}

}








