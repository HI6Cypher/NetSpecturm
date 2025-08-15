#ifndef UDP_H
#define UDP_H

#include "netrum.h"
#include "utils/hmem.h"
#include <stdlib.h>
#include <string.h>

#define UDP_HEADER_SIZE 0x8 * sizeof (char)
#define copy_udp_header_data(header, buf) copy_header_data(header, buf)

void split_udp(Frame *frame, unsigned char *buf);

#endif
