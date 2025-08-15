#include "core/split/tcp.h"

static Tags tags;
static Ports ports;

static void copy_tcp_next_header(Header *tcp) {
    /* application layer will be implemented after tests */
    tcp->next = DATA;
    return;
}

void split_tcp(Frame *frame, unsigned char *buf) {
    Header *tcp;
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    tcp = &frame->headers[frame->header_index];
    tcp->name = TCP;
    tcp->size = ((buf + frame->offset)[12] >> 4) * 4;
    copy_tcp_header_data(tcp, buf + frame->offset);
    copy_tcp_next_header(tcp);
    frame->offset += (unsigned int) tcp->size;
    return;
}
