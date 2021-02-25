#include "dpdk.h"
#include "dpdk_param.h"
#include "dpdk_port_conf.h"
#include <rte_ethdev.h>

/**
 *dpdk初始化，参数从配置文件中获取
 *
 * @param  config_file_path 配置文件路径
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
 *dpdk 端口初始化，端口信息从配置文件中填写
 *
 * @param  nb_mbuf 收发包数
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
 *dpdk 启动函数开始进行收发包
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
 *dpdk 停止函数，结束收发包
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
 *每调用一次返回一个初始化的网口和队列，用于多线程处理
 *
 * @param  *out_port
 * 	网口返回值
 * @param  *out_queue
 * 	队列返回值
 *   
 * @return 0成功 -1失败
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

