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
    size_t size = get_ipv4_header_size(buf + frame->buf_index);
    if (!new_header(frame, size)) {/* log */ frame->status = 0; return;}
    frame->headers[frame->header_index]->name = IPv4;
    frame->headers[frame->header_index]->size = size;
    copy_ipv4_header_data(frame->headers[frame->header_index], buf + frame->buf_index);
    copy_ipv4_proto(frame->headers[frame->header_index]);
    frame->buf_index += (unsigned int) size;
    return;
}
