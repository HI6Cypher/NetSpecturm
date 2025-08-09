#ifndef IPV6_H
#define IPV6_H

#include "netrum.h"
#include <stdlib.h>
#include <string.h>

#define IPv6_HEADER_SIZE 0x28 * sizeof (char)

void split_ipv6(Frame *frame, unsigned char *buf);

#endif
