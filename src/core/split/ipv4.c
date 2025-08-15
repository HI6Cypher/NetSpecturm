#include "core/split/ipv4.h"

static Tags tags;

static size_t get_ipv4_header_size(unsigned char *buf) {
    return (buf[0] & 0xf) * 4;
}

static void copy_ipv4_header_data(Header *ip, unsigned char *buf) {
    ip->data = malloc(ip->size);
    if (!ip->data) {/* log */ return;}
    memcpy(ip->data, buf, ip->size);
    return;
}

static void copy_ipv4_proto(Header *ip) {
    if (!ip->data) {ip->next = DATA; return;}
    ip->next = ip->data[9];
    return;
}

void split_ipv4(Frame *frame, unsigned char *buf) {
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    Header *ip = &frame->headers[frame->header_index];
    ip->name = IPv4;
    ip->size = get_ipv4_header_size(buf + frame->offset);
    copy_ipv4_header_data(ip, buf + frame->offset);
    copy_ipv4_proto(ip);
    frame->offset += ip->size;
    return;
}
