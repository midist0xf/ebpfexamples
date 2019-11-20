#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/in.h>
#include "bpf_helpers.h"
#include <stddef.h>
SEC("socket")
int bpfrog1(struct __sk_buff *skb)
{
	int proto = load_byte(skb, ETH_HLEN + offsetof(struct iphdr, protocol));
	int icmptype = load_byte(skb, ETH_HLEN + sizeof(struct iphdr) + offsetof(struct icmphdr, type));

	if (proto == IPPROTO_ICMP &&  icmptype == ICMP_ECHOREPLY)//skb->pkt_type == PACKET_OUTGOING)
		return -1;
	else 
		return 0;

}
char _license[] SEC("license") = "GPL";

