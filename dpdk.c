#include "dpdk.h"
#include "dpdk_param.h"
#include "dpdk_port_conf.h"
#include <rte_ethdev.h>

/**
 *dpdk��ʼ���������������ļ��л�ȡ
 *
 * @param  config_file_path �����ļ�·��
 * 	 
 * @return 
 *   
 */
void dpdk_init(char *config_file_path)
{
	int ret = 0;

	if (!config_file_path)	
		rte_exit(EXIT_FAILURE, "invalid config path !n");
	
	if((ret = dpdk_conf_parse(config_file_path)) != 0)	
			rte_exit(EXIT_FAILURE, "dpdk_conf_parse error!\n");
	
	int argc = dpdk_get_param_cnt();
	char **argv = dpdk_get_param();
	
	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Cannot init EAL\n");
}

/**
 *dpdk �˿ڳ�ʼ�����˿���Ϣ�������ļ�����д
 *
 * @param  nb_mbuf �շ�����
 * 	 
 * @return 
 *   
 */
void dpdk_port_setup(uint32_t nb_mbuf)
{
	dpdk_init_mem(nb_mbuf);
	dpdk_port_setup_proc();
}

/**
 *dpdk ����������ʼ�����շ���
 *
 * @param  
 * 	 
 * @return 
 *   
 */
void dpdk_start()
{
	int port_id,ret;
	
	RTE_ETH_FOREACH_DEV(port_id) {
	/* Start device */
	//printf("dpdk_port_start: port_id:%d\n",port_id);
	ret = rte_eth_dev_start(port_id);
	if (ret < 0)
		rte_exit(EXIT_FAILURE,"rte_eth_dev_start: err=%d, port=%d\n",ret, port_id);
	}
}


/**
 *dpdk ֹͣ�����������շ���
 *
 * @param  
 * 	 
 * @return 
 *   
 */
void dpdk_stop()
{
	int portid;
	RTE_ETH_FOREACH_DEV(portid) {
		rte_eth_dev_stop(portid);
		rte_eth_dev_close(portid);
		printf(" Done\n");
	}
	printf("Bye...\n");

}


/**
 *ÿ����һ�η���һ����ʼ�������ںͶ��У����ڶ��̴߳���
 *
 * @param  *out_port
 * 	���ڷ���ֵ
 * @param  *out_queue
 * 	���з���ֵ
 *   
 * @return 0�ɹ� -1ʧ��
 *   
 */
int dpdk_get_setup_port_and_queue(uint16_t *out_port,uint16_t *out_queue)
{
	static uint8_t first_time = 1;
	static uint16_t port = 0;
	static uint16_t queue = 0;

	int nr_ports = dpdk_get_port_num();
	int nr_queues  = dpdk_get_port_queue_num(port);

	if (first_time == 1)
	{
		first_time = 0;
		goto complete;
	}
	
	if ((queue + 1) < nr_queues)
		queue += 1;
	else {
		port += 1;
		queue = 0;
	}

	if(port >= nr_ports)
		return -1;
complete:
	*out_port = port;
	*out_queue = queue;
	return 0;

}

