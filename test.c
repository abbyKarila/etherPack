#include <net/ethernet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) {

	if (argc != 2) {
		printf("Usage: %s IPv4 address\n", argv[0]);
		exit(1);
	}
	
	int sock;				// raw socket
	char p[100] = "E\x00\x00\x14\x00\x01\x00\x00@\x00I2\x90\r\xd4\xe9\x90\r<\xb3";
	char *packet = p;

	/**** Create raw socket. ****/

	sock = socket(PF_INET, SOCK_RAW, htons(ETH_P_IP)); // PF_PACKET ETH_P_ALL
	if (sock < 0) {
		printf("Could not open raw socket, run as sudo!\n");
		exit(-1);
	}

	/**** Build IP address and port number. ****/

	struct sockaddr_in socketaddr;						// struct for address
	struct sockaddr_in *socketaddrp = &socketaddr;		// pointer to struct

	socketaddr.sin_family = AF_INET;					// protocol family (IP)
	socketaddr.sin_port = htons(9237);					// port number
	socketaddr.sin_addr.s_addr = inet_addr(argv[1]);

	/**** Connect. ****/

	if (connect(sock, (struct sockaddr *) socketaddrp, sizeof(struct sockaddr_in)) < 0) {
		printf("Could not connect.\n");
		printf("ERROR: %s\n", strerror(errno));
	}
	else printf("Connected!\n");
	
	/**** Send packet. ****/

	int sent = 0;					// total bytes sent
	int bytes;						// bytes sent this request
	
	while (sent < strlen(packet)) {	// loop until all bytes sent
		bytes = send(sock, p+sent, strlen(packet)-sent, 0);
		if (bytes == -1) {
			printf("Can't send.\n");
		}
		sent += bytes;				// update total bytes sent
	}
	
	printf("Sent: %d bytes\n", bytes);

	close(sock);
	printf("Socket closed!\n");

}
