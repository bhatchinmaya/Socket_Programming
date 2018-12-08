
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char  **argv)
{


   int clnt_socket = socket(AF_INET, SOCK_DGRAM, 0);
   if(clnt_socket == -1)
   { 
      perror("error in creating client socket");
   }

   printf("client socket created sucessfully\n");

   struct sockaddr_in clnt_addr, dest_addr;
   clnt_addr.sin_family = AF_INET;
   clnt_addr.sin_port = htons(3200);
   clnt_addr.sin_addr.s_addr = INADDR_ANY;



   

   //int clnt_send  = sendto(clnt_socket, "omsairam", 8, 0, (const struct sockaddr *)&clnt_addr, sizeof(clnt_addr));
   int clnt_send  = sendto(clnt_socket, argv[1], sizeof(argv[1]), 0, (const struct sockaddr *)&clnt_addr, sizeof(clnt_addr));
   if(clnt_send == -1)
   { 
      perror("error in sending client packet");
   }

   printf("send complete\nsent data is %s\n",argv[1] );
   }

  
