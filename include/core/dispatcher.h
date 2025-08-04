#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "tools/get_mtu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <mqueue.h>
#include <errno.h>
#ifdef __linux__
#include <linux/if_packet.h>
#endif

#define MTU_ERROR(mtu) \
    do {fprintf(stderr, "POS : inside start_sniffing()\nMSG : invalid mtu size %d\nERR : %d\n", mtu, errno); exit(EXIT_FAILURE);} while (0)
#define MQ_GETATTR_ERROR(val) \
    do {fprintf(stderr, "POS : inside check_queue_size()\nMSG : function mq_getattr() returns value %d\nERR : %d\n", val, errno); exit(EXIT_FAILURE);} while (0)
#define UNSUPPORTED_ETHERTYPE(type) \
    do {fprintf(stderr, "POS : inside start_sniffing()\nMSG : function get_ethertype() returns value %d\nERR : 0", type);} while (0)
#define MQ_SEND_ERROR(val) \
    do {fprintf(stderr, "POS : inside start_sniffing()\nMSG : function mq_send() returns value %d\nERR : %d", val, errno);} while (0)
#define QUEUE_SIZE_ERROR(len) \
    do {fprintf(stderr, "POS : inside start_sniffing()\nMSG : queue is full and packet with length %d ignored", len);} while (0)

unsigned int init_socket(unsigned char *iface);
unsigned int check_queue_size(mqd_t mqdes);
void start_sniffing(unsigned int sockfd, unsigned char *iface, mqd_t mqdes);

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

#endif
