#ifndef __DPDK___
#define __DPDK___
#include <stdio.h>
#include <stdint.h>

/**
 *dpdk��ʼ���������������ļ��л�ȡ
 *
 * @param  config_file_path �����ļ�·��
 * 	 
 * @return 
 *   
 */
void dpdk_init(char *config_file_path);

/**
 *dpdk ����������ʼ�����շ���
 *
 * @param  
 * 	 
 * @return 
 *   
 */
void dpdk_start();

/**
 *dpdk �˿ڳ�ʼ�����˿���Ϣ�������ļ�����д
 *
 * @param  nb_mbuf �շ�����
 * 	 
 * @return 
 *   
 */
void dpdk_port_setup();

/**
 *dpdk ֹͣ�����������շ���
 *
 * @param  
 * 	 
 * @return 
 *   
 */
void dpdk_stop();

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
int dpdk_get_setup_port_and_queue(uint16_t *out_port,uint16_t *out_queue);


#endif

