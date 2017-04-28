#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

// The packet length
#define PCKT_LEN 64

// Can create separate header file (.h) for all headers' structure
// The IP header's structure
struct ipheader {
 unsigned char      iph_ihl:5, iph_ver:4;
 unsigned char      iph_tos;
 unsigned short int iph_len;
 unsigned short int iph_ident;
 unsigned char      iph_flag;
 unsigned short int iph_offset;
 unsigned char      iph_ttl;
 unsigned char      iph_protocol;
 unsigned short int iph_chksum;
 unsigned int       iph_sourceip;
 unsigned int       iph_destip;
};

// UDP header's structure
struct udpheader {
 unsigned short int udph_srcport;
 unsigned short int udph_destport;
 unsigned short int udph_len;
 unsigned short int udph_chksum;
 };
// total udp header length: 8 bytes (=64 bits)

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

        if (argc != 5) {
                printf("Usage: sudo %s <source IPv4 address> <source port> <destination IPv4 address> <destination port>\n", argv[0]);
                exit(1);
        }

/**** Build IP address and port number. ****/

        int sock;                               // raw socket
        char buffer[PCKT_LEN];
		memset(buffer, 0, PCKT_LEN);
		
		struct ipheader *headerIP = (struct ipheader *) buffer;
		struct udpheader *headerUDP = (struct udpheader *) (buffer + sizeof(struct ipheader));
        struct sockaddr_in dstSockAddr;                                          // struct for address

        dstSockAddr.sin_family = AF_INET;                                        // protocol family (IP)
        dstSockAddr.sin_port = htons(atoi("9237"));                                      // port number
        dstSockAddr.sin_addr.s_addr = inet_addr(argv[3]);

		// use standard header structures but assign our own values.
		headerIP-> iph_ihl 		= 5;
		headerIP-> iph_ver 		= 4;
		headerIP-> iph_tos 		= 16; // Low delay
		headerIP-> iph_len 		= sizeof(struct ipheader) + sizeof(struct udpheader);
		headerIP-> iph_ident 	= htons(54321);
		headerIP-> iph_ttl 		= 64; // hops
		headerIP-> iph_protocol = 17; // UDP
		headerIP-> iph_sourceip = inet_addr(argv[1]);// Source IP address, can be spoofed
		headerIP-> iph_destip 	= inet_addr(argv[3]);// The destination IP address
		 
		// Fabricate the UDP header. Source port number, redundant
		headerUDP-> udph_srcport  = htons(atoi(argv[2]));
		headerUDP-> udph_destport = htons(atoi(argv[4]));// Destination port number
		headerUDP-> udph_len      = htons(sizeof(struct udpheader));
		
		// Calculate the checksum for integrity
		headerIP-> iph_chksum = calcCheckSum((unsigned short *)buffer, sizeof(struct ipheader) + sizeof(struct udpheader));
		
/**** Create raw socket. ****/

        sock = socket(PF_INET, SOCK_RAW, IPPROTO_UDP); // PF_PACKET, ,htons(ETH_P_ALL)
        if (sock < 0) {
                printf("Are you running as sudo?\n");
				perror("socket() error");
                exit(-1);
        }
		
		int i = 1;
        const int *val = &i;
		// Inform the kernel do not fill up the packet structure. we will build our own...
		if(setsockopt(sock, IPPROTO_IP, IP_HDRINCL, val, sizeof(i)) < 0)
		{
		perror("setsockopt() error");
		exit(-1);
		}

        // set send buffer size
        //setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&size, sizeof(size));

/**** Connect. ****/

        if (connect(sock, (struct sockaddr *) &dstSockAddr, sizeof(struct sockaddr_in)) < 0) {
                printf("Could not connect.\n");
                printf("ERROR: %s\n", strerror(errno));
        }
        else printf("Connected!\n");

/**** Send packet. ****/

		printf("Using Source IP: %s port: %u, Target IP: %s port: %u.\n", argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
        int bytes;                                              // bytes sent this request
		int count;
		for(count = 0; count < 20; count++) {
                bytes = send(sock, buffer, PCKT_LEN, 0);
                sleep(2);
                if( bytes < 0) {
					perror("sendto() error");
					exit(-1);
				}
				else {
					printf("Packet #%u sent.\n", count);
					sleep(2);
				}
        }
		
		printf("Sent %d packets, %d bytes each.\n", count, bytes);
        close(sock);
        printf("Socket closed!\n");

}
