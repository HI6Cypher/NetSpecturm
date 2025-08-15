#ifndef UNUSABLE_H
#define UNUSABLE_H

#include "netrum.h"
#include "utils/hmem.h"
#include <stdlib.h>
#include <string.h>

#define copy_data(header, buf) copy_header_data(header, buf)

void split_unusable(Frame *frame, unsigned char *buf);

#endif
