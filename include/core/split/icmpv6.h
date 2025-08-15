#ifndef ICMPV6_H
#define ICMPV6_H

#include "netrum"
#include "utils/hmem.h"
#include <stdlib.h>
#include <string.h>

void split_icmpv6(Frame *frame, unsigned char *buf);

#endif
