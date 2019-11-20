#include <stdio.h>
#include <assert.h>
#include <linux/bpf.h>
#include "libbpf.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <string.h>

#define PORT 55555

int main(int ac, char **argv)
{
	int sock, prog_fd;
	struct bpf_object *obj;
	char filename[256];

	if (bpf_prog_load("udpport_kern.o", BPF_PROG_TYPE_SOCKET_FILTER,
				&obj, &prog_fd))
		return 1;

	struct sockaddr_in servaddr;     

	memset(&servaddr, 0, sizeof(servaddr));

	/* fill address information */
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(PORT);

	/* create socket file descriptor for UDP protocol */
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fd < 0){
		printf("socket");
		return -1;
	}

	/* attach the filter */
	assert(setsockopt(fd, SOL_SOCKET, SO_ATTACH_BPF, &prog_fd,
				sizeof(prog_fd)) == 0);

	/* bind the local address to the socket */
	int n;
	n = bind(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (n < 0){
		printf("bind");
		return -1;
	}


	int i;
	char buf[65535];
	for (i = 0; i < 5; i++) {
		printf("Receiving...\n");
		int res = recvfrom(fd, buf, sizeof(buf), 0, NULL, 0);
		printf("size=%d\n", res);

		//sleep(5);
	}

	return 0;
}







