#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ojas");


static struct nf_hook_ops *nfho = NULL;

static unsigned int hfunc(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *iph;
    struct tcphdr *tcp_header;
    struct udphdr *udph;
    if (!skb)
        return NF_ACCEPT;

    iph = ip_hdr(skb);
	
    u32 src_ipa;
    src_ipa = ntohl(iph->saddr);

    if (iph->protocol == IPPROTO_TCP) {
        // printk(KERN_INFO "TCP packet detected!\n");

                tcp_header = (struct tcphdr *) skb_transport_header(skb);
	         /**
                 * SYN Scan Detected, drop packets
                 */
		if(tcp_header->syn && 
		   !(tcp_header->urg || tcp_header->ack || tcp_header->psh || tcp_header->rst || tcp_header->fin)){
			
			printk(KERN_INFO "SYN Scan detected! Src IP: %pI4h \n" ,&src_ipa);
			return NF_DROP;
		}
	    
                /**
                 * NULL Scan Detected, drop packets
                 */
                else if (!(tcp_header->syn || tcp_header->urg || tcp_header->ack || tcp_header->psh || tcp_header->rst || tcp_header->fin)) {
                        printk(KERN_INFO "NULL Scan detected! Src IP: %pI4h \n" ,&src_ipa);
                }

                /**
                 * ACK / Window Scan Detected, drop packets
                 */
                else if (tcp_header->ack && 
		   !(tcp_header->urg || tcp_header->syn || tcp_header->psh || tcp_header->rst || tcp_header->fin)) {

                        printk(KERN_INFO "ACK/Window Scan detected! Src IP: %pI4h \n" ,&src_ipa);
			return NF_DROP;
                }

                /**
                 * FIN Scan Detected, drop packets
                 */
                else if (tcp_header->fin && 
		   !(tcp_header->urg || tcp_header->ack || tcp_header->psh || tcp_header->rst || tcp_header->syn)) {

                        printk(KERN_INFO "FIN Scan detected! Src IP: %pI4h \n" ,&src_ipa);
			return NF_DROP;
                }

                /**
                 * XMAS Scan Detected , drop packets
                 */
                else if (tcp_header->fin &&  tcp_header->urg && tcp_header->psh && 
			 !(tcp_header->syn && tcp_header->rst && tcp_header->ack)) {
                        
			printk(KERN_INFO "XMAS Scan detected! Src IP: %pI4h \n" ,&src_ipa);
			return NF_DROP;
                }
	      /**
                 * If the packet is not of the above types, then accept
              */
	    return NF_ACCEPT;
    }else if (iph->protocol == IPPROTO_UDP) {
        printk(KERN_INFO "UDP packet detected!\n");
	    
                /**
                 * If the packet is destined to 53 port then only accept
                 */
		udph = udp_hdr(skb);
		if (ntohs(udph->dest) == 53) {
			return NF_ACCEPT;
		}
	}
        /**
         * Rest all type of connections are dropped
        */
    return NF_DROP;
}

static int __init LKM_init(void)
{
    nfho = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
    
    /* Initialize netfilter hook */
    nfho->hook  = (nf_hookfn*)hfunc;        /* hook function */
    nfho->hooknum   = NF_INET_PRE_ROUTING;      /* received packets */
    nfho->pf    = PF_INET;          /* IPv4 */
    nfho->priority  = NF_IP_PRI_FIRST;      /* max hook priority */
    
    nf_register_net_hook(&init_net, nfho);
    return 0;
}

static void __exit LKM_exit(void)
{
    nf_unregister_net_hook(&init_net, nfho);
    kfree(nfho);
}

module_init(LKM_init);
module_exit(LKM_exit);
