#ifndef NETRUM_H
#define NETRUM_H

#ifndef __linux__
#error "Unsupported Operation System, this Program is designed to run on Linux only"
#endif

#include <stdlib.h>
#include <stddef.h>
#include <time.h>

typedef enum {
    NONE = -0x0001,
    DATA = 0x0000,
    ICMPv4 = 0x0001,
    IGMP = 0x0002,
    IPIP = 0x0004,
    TCP = 0x0006,
    UDP = 0x0011,
    IPv6Encap = 0x0029,
    ICMPv6 = 0x003a,
    IPv6NoNxt = 0x003b,
    UDPLite = 0x0088,
    ETH = 0x00e1,
    ARP = 0x0806,
    IPv4 = 0x0800,
    IPv6 = 0x86dd,
} Tags;

typedef enum {
    FTPd = 20,
    FTPc = 21,
    SSH = 22,
    TELNET = 23,
    SMTP = 25,
    WHOIS = 43,
    DNS = 53,
    DHCPv4s = 67,
    DHCPv4c = 68,
    TFTP = 69,
    HTTP = 80,
    POP3 = 110,
    TIME = 123,
    IMAP = 143,
    HTTPS = 443,
    DHCPv6c = 546,
    DHCPv6s = 547,
    FTPSd = 989,
    FTPSc = 990,
    IMAPS = 993,
    POP3S = 995
} Ports;

typedef struct {
    signed int name;
    signed int  next;
    unsigned char *data;
    size_t size;
} Header;

typedef struct {
    unsigned short status : 1;
    unsigned short offset;
    unsigned short header_index;
    unsigned short header_count;
    unsigned int sequence;
    unsigned int ifindex;
    time_t time;
    size_t size;
    Header *headers;
} Frame;

extern unsigned char runstat;
extern signed int netrum_sockfd;
#endif
