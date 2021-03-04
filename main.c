#include "dpdk.h"

#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <rte_ethdev.h>

#define NB_MBUF             (8192 * 32) 
#define MAX_PKT_BURST 		(32)
#define PREFETCH_OFFSET	 	(3)
#define DPDK_CFG_PATH 		"./dpdk.cfg"
#define MAX_CORE  (8)
#define MAX_PORT  (2)
#define PERF_STAT (1) 
#define FORWARD (1)
#define LINE "-----------------------------------------------------------------"
struct dpdk_stat_t
{
	uint64_t rx_packets;
	uint64_t dropped_packets;
	uint64_t tx_packets;
	uint64_t resv[5];
}__attribute__((aligned(64)));

static struct dpdk_stat_t dpdk_stat[MAX_CORE][MAX_PORT]; 

volatile uint8_t running = 1;

void print_stat()
{
	struct dpdk_stat_t statics[MAX_PORT];
	int cpu = 0,inf = 0;

	memset(statics,0,sizeof(statics));
	for(cpu = 0; cpu < MAX_CORE; cpu++)
	{
		for(inf = 0; inf < MAX_PORT; inf++)
		{
			statics[inf].rx_packets += dpdk_stat[cpu][inf].rx_packets;
			statics[inf].tx_packets+= dpdk_stat[cpu][inf].tx_packets;
			statics[inf].dropped_packets+= dpdk_stat[cpu][inf].dropped_packets;
		}
	}
	printf("%s\n",LINE);	
	printf("%10s%18s%18s%18s \n","Interface","RX packets",
								 "TX packets ","Dropped packets");
	printf("%s\n",LINE);	

	for(inf = 0; inf < MAX_PORT; inf++)
	{
		printf("%10u%18llu%18llu%18llu \n",
		inf,
		statics[inf].rx_packets,
		statics[inf].tx_packets,
		statics[inf].dropped_packets);
	}
	printf("%s\n",LINE);	

	
	struct dpdk_stat_t statics_queue[MAX_CORE];
	memset(statics_queue,0,sizeof(statics_queue));
	
	for(cpu = 0; cpu < MAX_CORE; cpu++)
	{
		for(inf = 0; inf < MAX_PORT; inf++)
		{
			statics_queue[cpu].rx_packets += dpdk_stat[cpu][inf].rx_packets;
			statics_queue[cpu].tx_packets += dpdk_stat[cpu][inf].tx_packets;
			statics_queue[cpu].dropped_packets += dpdk_stat[cpu][inf].dropped_packets;
		}
		printf("cpu:%d rx_packets:%ld tx_packets:%ld drop:%ld\n",cpu,statics_queue[cpu].rx_packets,statics_queue[cpu].tx_packets,statics_queue[cpu].dropped_packets);

	}
	printf("%s\n",LINE);	
}

/* Custom handling of signals to handle process terminal */
static void
signal_handler(int signum)
{
	running = 0;
}

int dpdk_main_loop(void *dummy)
{
	int j,ret;
	uint16_t port = 0;
	uint16_t queue = 0;
	uint32_t lcore_id = rte_lcore_id();

	if(ret = dpdk_get_setup_port_and_queue(&port,&queue) != 0)
		rte_exit(EXIT_FAILURE, "dpdk_get_port_and_queue error!\n");
	
	uint16_t forward_port = (port % 2) ? (port - 1) :(port + 1);

	printf(" -- lcore_id=%u port_id=%u rxqueue_id=%hhu\n",lcore_id, port, queue);
	while(running){			
		struct rte_mbuf *pkts_burst[MAX_PKT_BURST];		
		const uint16_t nb_rx = rte_eth_rx_burst(port,queue, pkts_burst, MAX_PKT_BURST);
#ifdef PERF_STAT
		dpdk_stat[lcore_id][port].rx_packets += nb_rx;
#endif

#ifdef FORWARD
		uint16_t nb_tx = rte_eth_tx_burst(forward_port, queue, pkts_burst, nb_rx);
#ifdef PERF_STAT
		dpdk_stat[lcore_id][port].tx_packets += nb_tx;
		dpdk_stat[lcore_id][port].dropped_packets += (nb_rx - nb_tx);
#endif	
#else
		for (j = 0; j < PREFETCH_OFFSET && j < nb_rx; j++) 
			rte_prefetch0(rte_pktmbuf_mtod(pkts_burst[j], void *));

		/* Prefetch and forward already prefetched packets */
		for (j = 0; j < (nb_rx - PREFETCH_OFFSET); j++) {
			rte_prefetch0(rte_pktmbuf_mtod(pkts_burst[j + PREFETCH_OFFSET], void *));
			rte_pktmbuf_dump(stdout,pkts_burst[j],pkts_burst[j]->pkt_len);
			rte_pktmbuf_free(pkts_burst[j]);
		}

		/* Forward remaining prefetched packets */
		for (; j < nb_rx; j++) {		
			rte_pktmbuf_dump(stdout,pkts_burst[j],pkts_burst[j]->pkt_len);
			rte_pktmbuf_free(pkts_burst[j]);
		}

#endif
	}
	return 0;
}

int main(int argc, char **argv)
{
	int i, ret,lcore_id,portid;

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	//rte_eal_init(argc,argv);
	dpdk_init(DPDK_CFG_PATH);
	dpdk_port_setup(NB_MBUF);
#ifdef FORWARD
	if(dpdk_get_port_num() % 2)	
		rte_exit(EXIT_FAILURE, "port number is not even\n");
#endif
#ifdef PERF_STAT
	memset(dpdk_stat,0,sizeof(dpdk_stat));
#endif 

	dpdk_start();
	rte_eal_mp_remote_launch(dpdk_main_loop, NULL, CALL_MASTER);

	RTE_LCORE_FOREACH_SLAVE(lcore_id) 
		if (rte_eal_wait_lcore(lcore_id) < 0) 	
			rte_exit(EXIT_FAILURE, "rte_eal_wait_lcore exit\n");
	dpdk_stop();
	print_stat();
	printf("Bye...\n");
    return 0;
}

