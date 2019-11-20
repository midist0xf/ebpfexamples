#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/udp.h>
#include <linux/in.h>
#include "bpf_helpers.h"
#include <stddef.h>
#include <stdint.h>

struct bpf_map_def SEC("maps") my_map = {                              
	.type = BPF_MAP_TYPE_ARRAY,                                    
	.key_size = sizeof(uint32_t),                                  
	.value_size = sizeof(uint32_t), 
	.max_entries = 1,                                            
};


SEC("socket")
int bpfrog1(struct __sk_buff *skb)
{ 
	int index = 0;
	/* src port written from userspace in the map */
	uint32_t *srcport_usr = bpf_map_lookup_elem(&my_map, &index);
	if(srcport_usr){
		int srcport_pkt = load_half(skb, offsetof(struct udphdr, source));
		/* compare port numbers */
		if (srcport_pkt == (*srcport_usr)) 
			return -1; // return the entire packet
	}
        return 0; // discard the packet
}
char _license[] SEC("license") = "GPL";

