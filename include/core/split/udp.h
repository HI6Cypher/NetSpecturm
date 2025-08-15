#ifndef UDP_H
#define UDP_H

#include "netrum"
#include "utils/hmem.h"
#include <stdlib.h>
#include <string.h>

#define UDP_HEADER_SIZE 0x8 * sizeof (char)

split_udp(Frame *frame, unsigned char *buf);

#endif
