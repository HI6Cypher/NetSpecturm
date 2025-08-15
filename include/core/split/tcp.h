#ifndef TCP_H
#define TCP_H

#include "netrum.h"
#include "utils/hmem.h"
#include <stdlib.h>
#include <string.h>

#define copy_tcp_header_data(header, buf) copy_header_data(header, buf)

void split_tcp(Frame *frame, unsigned char *buf);

#endif
