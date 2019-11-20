// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <assert.h>
#include <linux/bpf.h>
#include "libbpf.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>

int main(int ac, char **argv)
{
	int sock, prog_fd;
	struct bpf_object *obj;
	char filename[256];

	if (bpf_prog_load("return_kern.o", BPF_PROG_TYPE_SOCKET_FILTER,
			  &obj, &prog_fd))
		return 1;

	/* open up a packet socket */
	sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));  
	if(sock < 0){
		printf("socket");
		return -1;
	}

	/* attach the filter */
	assert(setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, &prog_fd,
			  sizeof(prog_fd)) == 0);

	int i;
        char buf[65535];
	for (i = 0; i < 5; i++) {
	    printf("Receiving\n");
            int res = recvfrom(sock, buf, sizeof(buf), 0, NULL, 0);
            printf("size=%d\n", res);

	    //sleep(5);
	}

	return 0;
}







