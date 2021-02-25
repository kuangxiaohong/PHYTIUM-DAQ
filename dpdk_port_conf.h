#ifndef __DPDK_PORT_CONF__
#define __DPDK_PORT_CONF__

#include "stdint.h"

//can get from dpdk param
#define NB_SOCKETS        (8)

/**
 * numa�ܹ���Ϊÿ���˺Ͷ˿ڵ��������ڴ�ڵ��Ϸ����ڴ�
 *
 * @param nb_mbuf
 *   ����mbuf����
 * @return
 *   0 ��ʾ�ɹ�������Ϊʧ��
 */
int dpdk_init_mem(uint32_t nb_mbuf);


/**
 * ��ȡĳ���ڵĶ�����
 *
 * @param port_id
 *   ����id
 * @return �������ڶ�����
 *   
 */
int dpdk_get_port_queue_num(int port_id);

/**
 * ��ȡ������
 *
 * @param 
 *   
 * @return ����������
 *   
 */
int dpdk_get_port_num(void);



/**
 *��ӡ����������Ϣ
 *
 * @param  
 * 	 
 * @return 
 *   
 */
void dpdk_ports_print(void);

/**
 * �������link״̬
 *
 * @param port_num
 *   ���ڸ���
 * @param port_mask
 *   ��������
 * @return
 *   
 */
void check_all_ports_link_status(uint8_t port_num, uint32_t port_mask);

/**
 *��ȡ���ж��и���
 *
 * @param  
 * 	 
 * @return �������ж�����
 *   
 */
int dpdk_get_port_queue_total(void);


/**
 * ��������������Ϣ��ȷ�����ڸ����Ͷ����������������ڳ�ʼ��
 *
 * @param 
 *   
 * @return
 *   
 */
void dpdk_port_setup_proc(void);

#endif
