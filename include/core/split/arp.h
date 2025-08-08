#ifndef ARP_H
#define ARP_H

#include "netrum.h"
#include <stdlib>
#include <string.h>

#define ARPIP_HEADER_SIZE 0x1c * sizeof (char)

void split_arp(Frame *frame, unsigned char *buf);

#endif
