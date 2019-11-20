// SPDX-License-Identifier: GPL-2.0
#include <linux/bpf.h>
#include "libbpf.h"
#include <unistd.h>
#include <fcntl.h>

int main(int ac, char **argv)
{
	int prog_fd, fd;
	struct bpf_object *obj;

	if (bpf_prog_load("traceopenat_kern.o", BPF_PROG_TYPE_TRACEPOINT,
			  &obj, &prog_fd))
		return 1;
	struct bpf_program *prog = bpf_object__find_program_by_title(obj, "tracepoint/syscalls/sys_enter_openat");
	bpf_program__attach_tracepoint(prog, "syscalls", "sys_enter_openat");

        fd = open("mine_user.c", O_RDONLY);
	close(fd);
        //fork();

	return 0;
}

