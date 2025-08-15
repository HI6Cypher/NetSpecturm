#ifndef GET_MTU_H
#define GET_MTU_H

#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#ifdef __linux__
#include <linux/if.h>
#endif

unsigned int get_mtu_ifname(unsigned int sockfd, unsigned char *iface);
unsigned int get_mtu_ifindex(unsigned int sockfd, unsigned int ifindex);

#endif
