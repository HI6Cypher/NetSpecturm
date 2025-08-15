#ifndef ICMPV4_H
#define ICMPV4_H

#include "netrum.h"
#include "utils/hmem.h"
#include <stdlib.h>
#include <string.h>

void split_icmpv4(Frame *frame, unsigned char *buf);

#endif
