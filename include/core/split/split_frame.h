#ifndef SPLIT_FRAME_H
#define SPLIT_FRAME_H

#include "netrum.h"
#include "core/split/split_network.h"
#include <stdlib.h>
#include <string.h>

#define ETHERNET_HEADER_SIZE 0xe
#define MALLOC_ERROR(val) \
    do {fprintf(stderr, "POS : split_frame module copy_header_data()\nMSG : malloc returns %d\nERR : %d", val, errno);} while (0)

void split_frame(Frame *frame, unsigned char *buf);

#endif
