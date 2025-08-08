#ifndef NETRUM_H
#define NETRUM_H

#ifndef __linux__
#error "Unsupported Operation System, this Program is designed to run on Linux only"
#endif

#include <stdlib.h>
#include <stddef.h>
#include <time.h>

typedef enum {
    DATA = 0x0000
    ICMPv4 = 0x0001,
    IGMP = 0x0002,
    IPIP = 0x0004,
    TCP = 0x0006,
    UDP = 0x0011,
    IPv6Encap = 0x0029,
    ICMPv6 = 0x003a,
    IPv6NoNxt = 0x003b,
    SCTP = 0x0084,
    UDPLite = 0x0088
    ETH = 0x00e1,
    ARP = 0x0806,
    IPv4 = 0x0800,
    IPv6 = 0x86dd,
} Tags;

typedef struct {
    unsigned int name;
    unsigned int  next;
    unsigned char *data;
    size_t size;
} Header;

typedef struct {
    unsigned short status : 1;
    unsigned short buf_index;
    unsigned short header_index;
    unsigned short hnum;
    unsigned int sequence;
    unsigned int ifindex;
    time_t time;
    size_t size;
    Header *headers;
} Frame;

unsigned int halloc(Frame *frame);
void hfree(Frame *frame);
unsigned int new_header(Frame *frame, size_t header_size);

extern unsigned char runstat;
extern signed int netrum_sockfd;

#endif
