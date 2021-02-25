ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
RTE_TARGET ?= arm64-armv8a-linuxapp-gcc
APP = phytium_dpdk
CC = gcc
SRCS-y := main.c dpdk_param.c dpdk_port_conf.c dpdk.c
LDFLAGS_STATIC =  -D_GNU_SOURCE -pthread -Wl,--no-as-needed -Wl,-export-dynamic -Wl,-lrte_flow_classify -Wl,-lrte_pipeline -Wl,-lrte_table -Wl,-lrte_port -Wl,-lrte_pdump -Wl,-lrte_distributor -Wl,-lrte_ip_frag -Wl,-lrte_gro -Wl,-lrte_gso -Wl,-lrte_meter -Wl,-lrte_lpm -Wl,--whole-archive -Wl,-lrte_acl -Wl,--no-whole-archive -Wl,-lrte_jobstats -Wl,-lrte_metrics -Wl,-lrte_bitratestats -Wl,-lrte_latencystats -Wl,-lrte_power -Wl,-lrte_timer -Wl,-lrte_efd -Wl,--whole-archive -Wl,-lrte_cfgfile -Wl,-lrte_hash -Wl,-lrte_member -Wl,-lrte_vhost -Wl,-lrte_kvargs -Wl,-lrte_mbuf -Wl,-lrte_net -Wl,-lrte_ethdev -Wl,-lrte_cryptodev -Wl,-lrte_security -Wl,-lrte_eventdev -Wl,-lrte_mempool -Wl,-lrte_mempool_ring -Wl,-lrte_ring -Wl,-lrte_pci -Wl,-lrte_eal -Wl,-lrte_cmdline -Wl,-lrte_reorder -Wl,-lrte_sched -Wl,-lrte_kni -Wl,-lrte_bus_pci -Wl,-lrte_bus_vdev -Wl,-lrte_mempool_stack -Wl,-lrte_pmd_af_packet -Wl,-lrte_pmd_bond -Wl,-lrte_pmd_e1000 -Wl,-lrte_pmd_failsafe -Wl,-lrte_pmd_i40e -Wl,-lrte_pmd_ixgbe -Wl,-lrte_pmd_kni -Wl,-lrte_pmd_null -Wl,-lrte_pmd_ring -Wl,-lrte_pmd_softnic -Wl,-lrte_pmd_tap -Wl,-lrte_pmd_virtio -Wl,-lrte_pmd_vhost -Wl,-lrte_pmd_null_crypto -Wl,-lrte_pmd_crypto_scheduler -Wl,-lrte_pmd_skeleton_event -Wl,-lrte_pmd_sw_event -Wl,--no-whole-archive -Wl,-lrt -Wl,-lm -Wl,-lnuma -Wl,-ldl -Wl,-export-dynamic -Wl,-L/home/tools/dpdk-stable-19.11.5/arm64-armv8a-linuxapp-gcc/lib/ /home/tools/dpdk-stable-19.11.5/arm64-armv8a-linuxapp-gcc/lib/libdpdk.a

CFLAGS += -O3 -g -w -I$(PWD)/  -I$(RTE_SDK)/$(RTE_TARGET)/include

LDFLAGS += -lpthread

$(APP): $(SRCS-y) 
	$(CC) $(CFLAGS) $(SRCS-y) -o $@ $(LDFLAGS) $(LDFLAGS_STATIC) 

clean:
	rm -rf $(APP) *.o 