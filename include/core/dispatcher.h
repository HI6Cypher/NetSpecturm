#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "netrum.h"
#include "utils/terminate.h"
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

#define BACKUP_MTU 1500 + 14

typedef enum {
    IPv4_PRIO,
    ARP_PRIO,
    IPv6_PRIO
} Priority;

unsigned int init_socket(unsigned char *iface);
unsigned int check_queue_size(mqd_t qframe);
void start_sniffing(unsigned int sockfd, unsigned char *iface, mqd_t qframe, unsigned short *runstat);
void dispatcher(mqd_t qframe, unsigned char *iface, pthread_mutex_t *lock, unsigned short *runstat, unsigned int *sockfd);

#endif
