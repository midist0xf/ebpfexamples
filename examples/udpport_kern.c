#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/udp.h>
#include <linux/in.h>
#include "bpf_helpers.h"
#include <stddef.h>

SEC("socket")
int bpfrog1(struct __sk_buff *skb)
{
	int srcport = load_half(skb, offsetof(struct udphdr, source));
	if (srcport == 0x0406) //1030
		return -1; // return the entire packet
	else 
		return 0; // discard the packet
}
char _license[] SEC("license") = "GPL";

