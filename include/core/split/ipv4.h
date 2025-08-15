#ifndef IPV4_H
#define IPV4_H

#include "netrum.h"
#include "utils/hmem.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define copy_ipv4_header_data(header, buf) copy_header_data(header, buf)

void split_ipv4(Frame *frame, unsigned char *buf);

#endif
