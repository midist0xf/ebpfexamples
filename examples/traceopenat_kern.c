#include <linux/bpf.h>
#include "bpf_helpers.h"
#include <fcntl.h>

struct syscalls_enter_openat_args {
	__u64 pad;
	int __syscall_nr;
	const char * filename;
	int flags;
	unsigned short modep;
};
SEC("tracepoint/syscalls/sys_enter_openat")
int bpf_sys(struct syscalls_enter_openat_args *ctx)
{
	char fmt[] = "openat read only\n";
	int flags = ctx->flags;

        if ((flags & O_RDONLY) == O_RDONLY)
		bpf_trace_printk(fmt, sizeof(fmt));	
	return 0;
}
char _license[] SEC("license") = "GPL";

