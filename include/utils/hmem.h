#ifndef HMEM_H
#define HMEM_H

#include "netrum.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

unsigned int halloc(Frame *frame);
void hfree(Frame *frame);
unsigned int new_header(Frame *frame);
void copy_header_data(Header *header, unsigned char *buf);

#endif
