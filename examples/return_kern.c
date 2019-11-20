#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/in.h>
#include "bpf_helpers.h"
#include <stddef.h>
SEC("socket")
int bpf_sk_prog(struct __sk_buff *skb)
{
		// Discard all
		return 0;
		// Accept all
		// return -1;

}
char _license[] SEC("license") = "GPL";

