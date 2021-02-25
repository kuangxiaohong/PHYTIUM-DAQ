#ifndef __DPDK_PORT_CONF__
#define __DPDK_PORT_CONF__

#include "stdint.h"

//can get from dpdk param
#define NB_SOCKETS        (8)

/**
 * numa架构下为每个核和端口到其所在内存节点上分配内存
 *
 * @param nb_mbuf
 *   分配mbuf个数
 * @return
 *   0 表示成功，其它为失败
 */
int dpdk_init_mem(uint32_t nb_mbuf);


/**
 * 获取某网口的队列数
 *
 * @param port_id
 *   网口id
 * @return 返回网口队列数
 *   
 */
int dpdk_get_port_queue_num(int port_id);

/**
 * 获取网口数
 *
 * @param 
 *   
 * @return 返回网口数
 *   
 */
int dpdk_get_port_num(void);



/**
 *打印所有网口信息
 *
 * @param  
 * 	 
 * @return 
 *   
 */
void dpdk_ports_print(void);

/**
 * 检测网口link状态
 *
 * @param port_num
 *   网口个数
 * @param port_mask
 *   网口掩码
 * @return
 *   
 */
void check_all_ports_link_status(uint8_t port_num, uint32_t port_mask);

/**
 *获取所有队列个数
 *
 * @param  
 * 	 
 * @return 返回所有队列数
 *   
 */
int dpdk_get_port_queue_total(void);


/**
 * 根据网口配置信息来确定网口个数和队列数，并进行网口初始化
 *
 * @param 
 *   
 * @return
 *   
 */
void dpdk_port_setup_proc(void);

#endif
