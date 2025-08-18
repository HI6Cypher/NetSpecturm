#include "core/split/ipv4.h"

static Tags tags;

static size_t get_ipv4_header_size(unsigned char *buf) {
    return (buf[0] & 0xf) * 4;
}

static void copy_ipv4_proto(Header *ip) {
    if (!ip->data) {ip->next = DATA; return;}
    ip->next = ip->data[9];
    return;
}

void split_ipv4(Frame *frame, unsigned char *buf) {
    Header *ip;
    if (!new_header(frame)) {
        LOG("WARNN", "ipv4.c", "split_ipv4()", "Unable to allocate new header for IPv4 with offset %d", frame->offset);
        frame->status = 1;
        frame->error = 1;
        return;
    }
    ip = &frame->headers[frame->header_index];
    ip->name = IPv4;
    ip->size = get_ipv4_header_size(buf + frame->offset);
    copy_ipv4_header_data(ip, buf + frame->offset);
    copy_ipv4_proto(ip);
    frame->offset += ip->size;
    return;
}
