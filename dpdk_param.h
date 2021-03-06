#ifndef __DPDK_PARAM__
#define __DPDK_PARAM__

typedef struct dpdk_port_conf
{
	int queue_num;
	int mtu;
	int jumbo;
}dpdk_port_conf_t;

/**
 *dpdk配置文件解析
 *
 * @param  config_file_path 配置文件路径
 * 	 
 * @return 0成功，其它失败
 *   
 */
int dpdk_conf_parse(char *config_file_path);

/**
 *dpdk配置参数个数获取
 *
 * @param  
 * 	 
 * @return 返回配置参数个数
 *   
 */
int dpdk_get_param_cnt(void);

/**
 *dpdk配置参数获取
 *
 * @param  
 * 	 
 * @return 返回配置参数
 *   
 */
char **dpdk_get_param(void);

/**
 *dpdk网口配置个数获取
 *
 * @param  
 * 	 
 * @return 返回网口配置个数
 *   
 */
int dpdk_get_port_cnt(void);

/**
 *dpdk网口配置信息获取
 *
 * @param  
 * 	 
 * @return 返回某个网口配置信息
 *   
 */
dpdk_port_conf_t* dpdk_get_port_conf(int port_id);

#endif

