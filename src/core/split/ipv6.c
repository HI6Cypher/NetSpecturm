#include "core/split/ipv6.h"

static Tags tags;

static void copy_ipv6_header_data(Header *ip, unsigned char *buf) {
    ip->data = (Header *) malloc(ip->size);
    if (!ip->data) {/* log */ return;}
    memcpy(ip->data, buf, ip->size);
    return;
}

static void copy_ipv6_next_header(Header *ip) {
    if (!ip->data) {ip->next = DATA; return;}
    ip->next = ip->data[6];
    return;
}

void split_ipv6(Frame *frame, unsigned char *buf) {
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    frame->headers[frame->header_index]->name = IPv6;
    frame->headers[frame->header_index]->size = IPv6_HEADER_SIZE;
    copy_ipv6_header_data(frame->headers[frame->header_index], buf + frame->offset);
    copy_ipv6_next_header(frame->headers[frame->header_index]);
    frame->offset += (unsigned int) IPv6_HEADER_SIZE;
    return;
}
