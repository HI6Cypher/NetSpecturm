#ifndef NETRUM_H
#define NETRUM_H

#ifndef __linux__
#error "Unsupported Operation System, this Program is designed to run on Linux only"
#endif

typedef enum {
    DATA = 0x0000
    ICMPv4 = 0x0001,
    IGMP = 0x0002,
    IPIP = 0x0004,
    TCP = 0x0006,
    UDP = 0x0011,
    IPv6C = 0x0029,
    ICMPv6 = 0x003a,
    IPv6NX = 0x003b,
    SCTP = 0x0084,
    UDPL = 0x0088
    ETH = 0x00e1,
    ARP = 0x0806,
    IPv4 = 0x0800,
    IPv6 = 0x86dd,
} Tags;

typedef struct {
    unsigned char proto_tag;
    unsigned char next_proto_tag;
    size_t header_size;
    unsigned char *header_data;
} Format;

typedef Format Datalink;

typedef Format Network;

typedef Format Transport;

typedef Format Application;

typedef struct {
    size_t data_size;
    unsigned char *data;
} Unusable;

typedef struct {
    Datalink link;
    Network net[2];
    Transport trans;
    Application app;
    Unusable data;
    unsigned int com
} Frame;

#endif
