#ifndef IPV4_H
#define IPV4_H

#include "netrum.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void split_ipv4(Frame *frame, unsigned int *buf);

#endif
