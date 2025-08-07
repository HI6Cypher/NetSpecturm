#ifndef SPLIT_NETWORK_H
#define SPLIT_NETWORK_H

#include "netrum.h"
#include "core/split/split_transport.h"
#include <stdlib.h>
#include <string.h>

#define ARPIP_HEADER_SIZE 0x1c

void split_network(Frame *frame, unsigned char *buf);
void split_network_arp(Frame *frame, unsigned char *buf);
void split_network_ipv4(Frame *frame, unsigned char *buf);
void split_network_ipv6(Frame *frame, unsigned char *buf);

#endif
