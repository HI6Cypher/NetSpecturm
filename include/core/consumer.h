#ifndef CONSUMER_H
#define CONSUMER_H

#include "netrum.h"
#include <stddef.h>
#include <string.h>
#include <mqueue.h>
#include <errno.h>

#define UNSUPPORTED_ETHERTYPE(val) \
    do {fprintf(stderr, "POS : copy_ethernet_tags()\nMSG : ethertype with value %d\nERR : 0", val);} while (0)
#define MALLOC_ERROR(val) \
    do {fprintf(stderr, "POS : copy_ethernet_header_data()\nMSG : malloc returns %d\nERR : %d", val, errno);} while (0)
#define UNSUPPORTED_NETPROTO(val) \
    do {fprintf(stderr, "POS : copy_network_header_size()\nMSG :  %d\nERR : 0", val);} while (0)

void splitter(Frame *frame, unsigned char *buf);
void handle_output(Frame *frame);
void consumer();

#endif
