#ifndef ARP_H
#define ARP_H

#include "netrum.h"
#include "utils/hmem.h"
#include <stdlib.h>
#include <string.h>

#define ARPIP_HEADER_SIZE 0x1c * sizeof (char)
#define copy_arp_header_data(header, buf) copy_header_data(header, buf)

void split_arp(Frame *frame, unsigned char *buf);

#endif
