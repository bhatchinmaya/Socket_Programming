
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>

#define SIZE 70000
#define PORT 3200
int sock_raw;
int tcp=0, udp=0, icmp=0, other=0;


int main(int argc, char  **argv){

	int saddr_size, pkt_bt;
	struct sockaddr saddr;
	struct in_addr in;
	struct sockaddr_ll sll;
	struct ifreq ifr;
	struct sockaddr_in source, dest;
	char buff[SIZE];
	char src[16];
	char dst[16];
	sock_raw = socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(sock_raw == -1){
		perror("FAILED:create socket");
	}

	strcpy(ifr.ifr_name,"eth0");
	int prom = ioctl(sock_raw,SIOCGIFFLAGS,&ifr);
	if(prom == -1){
		perror("FAILED:promiscuous failed");
	}
	sll.sll_family = AF_PACKET;
	sll.sll_ifindex = ifr.ifr_ifindex;
	sll.sll_protocol = htons(ETH_P_ALL);

	int sock_bind = bind(sock_raw, (struct sockaddr*)&sll, sizeof(sll));
	if(sock_bind == -1){
		perror("FAILED:bind");
	}

	struct ether_header *eth = (struct ether_header *)buff;
	struct iphdr *iph = (struct iphdr*) (buff+sizeof(struct ether_header));
	struct udphdr *udph = (struct udphdr *)(buff+ sizeof(struct iphdr) + sizeof(struct ether_header));
	struct tcphdr *tcph = (struct tcphdr *)(buff+ sizeof(struct iphdr) + sizeof(struct ether_header));
	struct icmphdr *icmph = (struct icmphdr *)(buff+ sizeof(struct iphdr)+sizeof(struct ether_header));

	while(1){
		saddr_size = sizeof(sll);
		pkt_bt = recvfrom(sock_raw, buff, sizeof(buff), 0,(struct sockaddr*)&sll,&saddr_size);
		if(pkt_bt == -1){
			perror("FAILED:recvfrom");
		}
		else{
			if( ntohs(eth->ether_type) == 0x0800){
				inet_ntop(AF_INET, &iph->saddr, src,16);
				inet_ntop(AF_INET, &iph->daddr, dst,16);

				switch(iph->protocol){
					case 1:
						//icmp = icmp+1;
						printf("ICMP: SRC addr:%-27s DST addr:%-16s \n",src,dst);
						break;
					case 6:
						//tcp = tcp+1;
						printf("TCP : SRC addr:%-16s port:%-6u DST addr:%-16s port:%-6u\n",src,ntohs(tcph->source),dst,ntohs(tcph->dest));
						break;
					case 17:
						//udp = udp+1;
						printf("UDP : SRC addr:%-16s port:%-6u DST addr:%-16s port:%-6u\n",src,ntohs(udph->source),dst,ntohs(udph->dest));
					default:
						//other = other+1;
						break;
				}
			}
		}

	}//while
	
	close(sock_raw);

}

