#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h> 
#include <stdint.h>
#include <time.h>
#include "chsum.h"

#define PING_SLEEP_RATE 1000000 

#define RECV_TIMEOUT 1  
#define IP4_HDRLEN 20




int main(int argc, char **argv) {

	if (argc != 2)
	{
		printf("input format: sudo %s <ip address>\n", argv[0]);
		exit(1);
	}
	int bytes = 64;
	int seq_num;
	memset(&seq_num, 0, sizeof(seq_num));
	uint16_t seq_counter = 0;	
	struct timespec time_start, time_end;
	long double rtt_msec=0, total_msec=0;
	struct timeval tv_out;
	tv_out.tv_sec = RECV_TIMEOUT; 
        tv_out.tv_usec = 0;

 
	int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
 	if (raw_socket == -1)
	{
		perror("error in creating socket");
	}

	int optval = 1;
  	setsockopt(raw_socket, SOL_IP, IP_TTL, &bytes , sizeof(bytes));

	struct sockaddr_in serveraddr, clntaddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);
	serveraddr.sin_port = htons(3200); 


while(1){

	seq_counter++;
	struct icmp icmp_header;
	memset(&icmp_header,0, sizeof(icmp_header));
// ICMP header
	icmp_header.icmp_type = ICMP_ECHO;
	icmp_header.icmp_code = 0;
	icmp_header.icmp_seq = seq_counter;
	uint8_t count = sizeof(icmp_header);  // 'chsum' fn takes "count" bytes beginning at location "addr". Refer RFC 1071.
	uint8_t *addr = &icmp_header.icmp_type;
	icmp_header.icmp_cksum = (chsum(addr, count));



// send and receive 
	usleep(PING_SLEEP_RATE); 
	clock_gettime(CLOCK_MONOTONIC, &time_start); 
	int send_data = sendto(raw_socket ,(const void *)&icmp_header, (size_t)sizeof(icmp_header), 0, (struct sockaddr *)&serveraddr
		, sizeof(serveraddr));
	if(send_data == -1)
	{
   		perror("send_data");
   		exit(1); 
  	}

	char recv_buffer[1000];
	memset(recv_buffer, 0, 1000); 
 	int len = sizeof(clntaddr);
	 
	int recv_data = recvfrom(raw_socket, recv_buffer, 1000, 0, ( struct sockaddr *)&serveraddr, &len);
	if(recv_data == -1)
	{
   		perror("recv_data");
   		exit(1); 
  	}
	clock_gettime(CLOCK_MONOTONIC, &time_end);

	struct ip *ipaddr = (struct ip*) recv_buffer;
	char *dest_ip = (char*)inet_ntoa(*(struct in_addr*)&ipaddr->ip_dst);
	char *src_ip =  (char*)inet_ntoa(*(struct in_addr*)&ipaddr->ip_src);
	unsigned char ttl = ipaddr->ip_ttl;
	unsigned short ip_length = ipaddr->ip_len;
 
	struct icmp *icmpaddr = (struct icmp *)(recv_buffer + IP4_HDRLEN); 
	unsigned short type = icmpaddr->icmp_type;
	seq_num = icmpaddr->icmp_seq;

	rtt_msec = (time_end.tv_sec - time_start.tv_sec) ;
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%e\n", bytes, src_ip, seq_num, ttl,  rtt_msec);



	 
	}

}






     

