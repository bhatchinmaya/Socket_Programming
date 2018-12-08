
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char  **argv)
{

  
  int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
   if(server_socket == -1)
   { 
      perror("error in creating server socket");
   }
   
   printf("server socket created successfully \n");

   char recv_buffer[25];
   void *ptr = &recv_buffer;
   
   struct sockaddr_in server_addr, clnt_addr;
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(3200);
   server_addr.sin_addr.s_addr = INADDR_ANY;


   int server_bind = bind(server_socket,(struct sockaddr*)&server_addr, sizeof(server_addr));
   if(server_bind == -1)
   { 
      perror("error in binding server socket");
   }


   printf("bind succssfull \n");
   //int server_recv = recvfrom(server_socket, ptr, 8, 0, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr));
   int server_recv = recvfrom(server_socket, ptr, 8, 0, NULL, 0);
   if(server_recv == -1)
   { 
      perror("error in receiving");
   }
   printf("data received from client is %s \n", recv_buffer);

}
