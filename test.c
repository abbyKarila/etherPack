#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

// The minimum ethernet frame size
#define PCKT_LEN 64
// 18 free bytes after headers and FCS
#define PAYLOAD_LEN 18

// UDP header structure
struct udpheader {
 unsigned short int udph_srcport;
 unsigned short int udph_destport;
 unsigned short int udph_len;
 unsigned short int udph_chksum;
 };
// total udp header length: 8 bytes

// UDP payload
struct udppayload {
 uint8_t octet[PAYLOAD_LEN];
 };

// RFC:
//  "The checksum field is the 16 bit one's complement of the one's
//  complement sum of all 16 bit words in the header.  For purposes of
//  computing the checksum, the value of the checksum field is zero."
unsigned short calcCheckSum(unsigned short *buf, int nwords) {
        unsigned long sum;
        for(sum=0; nwords>0; nwords--)
                sum += *buf++;
        sum = (sum >> 16) + (sum &0xffff);
        sum += (sum >> 16);
        return (unsigned short)(~sum);
}

int main(int argc, char** argv) {

        if (argc != 6) {
                printf("Usage: sudo %s <source IPv4 address> <source port> <destination IPv4 address> <destination port>\n", argv[0]);
                exit(1);
        }
		
		unsigned int mac[6];
		sscanf(argv[5], "%02x:%02x:%02x:%02x:%02x:%02x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

/**** Create raw socket. ****/

        int sock = socket(PF_PACKET, SOCK_RAW, IPPROTO_RAW); // htons(ETH_P_ALL)
        if (sock < 0) {
                printf("Are you running as sudo?\n");
				perror("socket() error");
                exit(-1);
        }

        // set send buffer size
        //setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&size, sizeof(size));
		
/**** Define interface for socket address. ****/

		struct sockaddr_ll socket_address;
		struct ifreq iface;
		memset(&iface, 0, sizeof(struct ifreq));
        strncpy(iface.ifr_name, "eth0", IFNAMSIZ-1);
		if (ioctl(sock, SIOCGIFINDEX, &iface) < 0) { perror("SIOCGIFINDEX"); }
		socket_address.sll_ifindex = iface.ifr_ifindex;

/**** Build packet. ****/
		
        char buffer[PCKT_LEN];
		memset(buffer, 0, PCKT_LEN);
		//memset(buffer + PCKT_LEN - sizeof(MAGIC_NUMBER), MAGIC_NUMBER, PCKT_LEN);
		
		struct ether_header *headerETH  = (struct ether_header *) buffer;
		struct iphdr 		*headerIP   = (struct iphdr *) 		 (buffer + sizeof(struct ether_header));
		struct udpheader 	*headerUDP  = (struct udpheader *)   (buffer + sizeof(struct ether_header) + sizeof(struct iphdr));
		struct udppayload 	*payloadUDP = (struct udppayload *)  (buffer + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udpheader));
        
    /* Ethernet header */
		/* Source MAC */
        headerETH-> ether_shost[0] = ((uint8_t *)&iface.ifr_hwaddr.sa_data)[0];
        headerETH-> ether_shost[1] = ((uint8_t *)&iface.ifr_hwaddr.sa_data)[1];
        headerETH-> ether_shost[2] = ((uint8_t *)&iface.ifr_hwaddr.sa_data)[2];
        headerETH-> ether_shost[3] = ((uint8_t *)&iface.ifr_hwaddr.sa_data)[3];
        headerETH-> ether_shost[4] = ((uint8_t *)&iface.ifr_hwaddr.sa_data)[4];
        headerETH-> ether_shost[5] = ((uint8_t *)&iface.ifr_hwaddr.sa_data)[5];
		/* Destination MAC */
        headerETH-> ether_dhost[0] = mac[0];
        headerETH-> ether_dhost[1] = mac[1];
        headerETH-> ether_dhost[2] = mac[2];
        headerETH-> ether_dhost[3] = mac[3];
        headerETH-> ether_dhost[4] = mac[4];
        headerETH-> ether_dhost[5] = mac[5];
        /* Ethertype */
        headerETH-> ether_type = htons(ETH_P_IP);
		
	/* IP header */
		headerIP-> ihl 		= 5;
		headerIP-> version 	= 4;
		headerIP-> tos 		= 16; // Low delay
		headerIP-> tot_len 	= htons( sizeof(struct iphdr) + sizeof(struct udpheader) + sizeof(struct udppayload));
		headerIP-> id 		= htons(54321);
		headerIP-> ttl 		= 64; // hops
		headerIP-> protocol = 17; // UDP
		headerIP-> saddr 	= inet_addr(argv[1]); // IP addresses can be spoofed
		headerIP-> daddr 	= inet_addr(argv[3]);
		 
	/* UDP header */
		headerUDP-> udph_srcport  = htons(atoi(argv[2]));
		headerUDP-> udph_destport = htons(atoi(argv[4]));
		headerUDP-> udph_len      = htons(sizeof(struct udpheader) + sizeof(struct udppayload));
		
	/* UDP payload */
		char b[] = "OSKAR WRITES CODE!";		
		for (int i=0; i<PAYLOAD_LEN; i++) {
			payloadUDP-> octet[i] = b[i];
		}
		
	/* Calculate IP checksum */
		headerIP-> check = calcCheckSum((unsigned short *)buffer, sizeof(struct iphdr) + sizeof(struct udpheader));
		
	/* Calculate FCS */
		
		
/**** Send packet. ****/

		//printf("Using Source IP: %s port: %u, Target IP: %s port: %u.\n", argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
        int bytes;
		int count;
		for( count=0; count<10; count++ ) {
                bytes = sendto(sock, buffer, PCKT_LEN, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
                //sleep(2);
                if( bytes != PCKT_LEN) {
					perror("sendto() error");
					exit(-1);
				}
				else {
					//printf("Packet #%u sent.\n", count);
				}
        }
		printf("Sent %d packets, %d bytes each.\n", count, bytes);
        close(sock);

}
