#ifndef IPV6_H
#define IPV6_H

#include "netrum.h"
#include "utils/hmem.h"
#include "utils/log_tool.h"
#include <stdlib.h>
#include <string.h>

#define copy_ipv6_header_data(header, buf) copy_header_data(header, buf)

#define IPv6_HEADER_SIZE 0x28 * sizeof (char)

void split_ipv6(Frame *frame, unsigned char *buf);

#endif
