#ifndef ICMPV6_H
#define ICMPV6_H

#include "netrum.h"
#include "utils/hmem.h"
#include <stdlib.h>
#include <string.h>

#define copy_icmpv6_header_data(header, buf) copy_header_data(header, buf)

void split_icmpv6(Frame *frame, unsigned char *buf);

#endif
