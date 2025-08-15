#include "core/split/ipv6.h"

static Tags tags;

static void copy_ipv6_header_data(Header *ip, unsigned char *buf) {
    copy_header_data(ip, buf);
    return;
}

static void copy_ipv6_next_header(Header *ip) {
    if (!ip->data) {ip->next = DATA; return;}
    ip->next = ip->data[6];
    return;
}

void split_ipv6(Frame *frame, unsigned char *buf) {
    Header *ip;
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    ip = &frame->headers[frame->header_index];
    ip->name = IPv6;
    ip->size = IPv6_HEADER_SIZE;
    copy_ipv6_header_data(ip, buf + frame->offset);
    copy_ipv6_next_header(ip);
    frame->offset += (unsigned int) IPv6_HEADER_SIZE;
    return;
}
