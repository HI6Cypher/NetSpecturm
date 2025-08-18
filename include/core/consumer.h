#ifndef CONSUMER_H
#define CONSUMER_H

#include "netrum.h"
#include "core/split/arp.h"
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <mqueue.h>
#include <errno.h>

#define ETH_HEADER_SIZE 14 * sizeof (char)

void splitter(Frame *frame, unsigned char *buf);
void handle_output(Frame *frame);
void consumer(mqd_t mqdes, signed int sockfd, unsigned char *iface);

#endif
