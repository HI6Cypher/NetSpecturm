#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "tools/get_mtu.h"
#include <stdlib.h>
#include <stdio.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <mqueue.h>
#include <errno.h>
#include <linux/if_packet.h>

#define MTU_ERROR(val) \
    do {fprintf(stderr, "POS : inside start_sniffing()\nMSG : invalid mtu size %d\nERR : %d\n", val, errno); exit(EXIT_FAILURE);} while (0)
#define MQ_GETATTR_ERROR(val) \
    do {fprintf(stderr, "POS : inside check_queue_size()\nMSG : function mq_getattr() returns value %d\nERR : %d\n", val, errno); exit(EXIT_FAILURE);} while (0)
#define UNSUPPORTED_ETHERTYPE(val) \
    do {fprintf(stderr, "POS : inside start_sniffing()\nMSG : function get_ethertype() returns value %d\nERR : 0", val);} while (0)
#define MQ_SEND_ERROR(val) \
    do {fprintf(stderr, "POS : inside start_sniffing()\nMSG : function mq_send() returns value %d\nERR : %d", val, errno);} while (0)
#define QUEUE_SIZE_ERROR(len) \
    do {fprintf(stderr, "POS : inside start_sniffing()\nMSG : queue is full and packet with length %d ignored", len);} while (0)
#define IFINDEX_ERROR(val) \
    do {fprintf(stderr, "POS : inside init_sockaddr_ll_structure()\nMSG : if_nametoindex faileds on arg %s\nERR : errno", val, errno); exit(EXIT_FAILURE);} while (0)

typedef enum {
    IPv4_PRIO,
    ARP_PRIO,
    IPv6_PRIO
} Priority;

typedef enum {
    IPv4 = 0x0800,
    IPv6 = 0x86dd,
    ARP = 0x0806
} Ethertype;

unsigned int init_socket(unsigned char *iface);
unsigned int check_queue_size(mqd_t mqdes);
void start_sniffing(unsigned int sockfd, unsigned char *iface, mqd_t mqdes);

#endif
