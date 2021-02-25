#ifndef __DPDK___
#define __DPDK___
#include <stdio.h>
#include <stdint.h>

/**
 *dpdk初始化，参数从配置文件中获取
 *
 * @param  config_file_path 配置文件路径
 * 	 
 * @return 
 *   
 */
void dpdk_init(char *config_file_path);

/**
 *dpdk 启动函数开始进行收发包
 *
 * @param  
 * 	 
 * @return 
 *   
 */
void dpdk_start();

/**
 *dpdk 端口初始化，端口信息从配置文件中填写
 *
 * @param  nb_mbuf 收发包数
 * 	 
 * @return 
 *   
 */
void dpdk_port_setup();

/**
 *dpdk 停止函数，结束收发包
 *
 * @param  
 * 	 
 * @return 
 *   
 */
void dpdk_stop();

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
int dpdk_get_setup_port_and_queue(uint16_t *out_port,uint16_t *out_queue);


#endif

