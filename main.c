#include "dpdk_param.h"
#include "dpdk_port_conf.h"

#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <rte_ethdev.h>


#define MAX_PKT_BURST (32)
#define PREFETCH_OFFSET	  (3)

volatile uint8_t running = 1;
/* Custom handling of signals to handle process terminal */
static void
signal_handler(int signum)
{
	uint16_t portid;
	running = 0;

	/* When we receive a SIGINT signal */
	if (signum == SIGINT) {
		RTE_ETH_FOREACH_DEV(portid) 
			rte_eth_dev_close(portid);
	}
	rte_exit(EXIT_SUCCESS, "\n User forced exit\n");
}

int dpdk_main_loop(void *dummy)
{
	int j,ret;
	uint16_t port = 0;
	uint16_t queue = 0;
	uint32_t lcore_id = rte_lcore_id();

	if(ret = dpdk_get_port_and_queue(&port,&queue) != 0)
		rte_exit(EXIT_FAILURE, "dpdk_get_port_and_queue error!\n");

	printf(" -- lcore_id=%u port_id=%u rxqueue_id=%hhu\n",lcore_id, port, queue);
	while(running){		
		struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
		const uint16_t nb_rx = rte_eth_rx_burst(port,queue, pkts_burst, MAX_PKT_BURST);

		for (j = 0; j < PREFETCH_OFFSET && j < nb_rx; j++) 
			rte_prefetch0(rte_pktmbuf_mtod(pkts_burst[j], void *));

		/* Prefetch and forward already prefetched packets */
		for (j = 0; j < (nb_rx - PREFETCH_OFFSET); j++) {
			rte_prefetch0(rte_pktmbuf_mtod(pkts_burst[
			j + PREFETCH_OFFSET], void *));

			rte_pktmbuf_dump(stdout,pkts_burst[j],pkts_burst[j]->pkt_len);
			rte_pktmbuf_free(pkts_burst[j]);
		}

		/* Forward remaining prefetched packets */
		for (; j < nb_rx; j++) {			
			rte_pktmbuf_dump(stdout,pkts_burst[j],pkts_burst[j]->pkt_len);
			rte_pktmbuf_free(pkts_burst[j]);
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	int i, ret,lcore_id,portid;

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	
	if(ret = dpdk_conf_parse() != 0)	
		rte_exit(EXIT_FAILURE, "conf parse error!\n");

	argc = dpdk_get_param_cnt();
	argv = dpdk_get_param();

	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Cannot init EAL\n");
	dpdk_port_setup();
	dpdk_port_start();

	rte_eal_mp_remote_launch(dpdk_main_loop, NULL, CALL_MASTER);

	RTE_LCORE_FOREACH_SLAVE(lcore_id) 
		if (rte_eal_wait_lcore(lcore_id) < 0) 	
			rte_exit(EXIT_FAILURE, "rte_eal_wait_lcore exit\n");

	printf("Bye...\n");
    return 0;
}

