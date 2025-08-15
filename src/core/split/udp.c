#include "core/split/udp.h"

static Tags tags;
static Ports ports;

static void copy_udp_next_header(Header *udp) {
    /* applicatin layer will be implemented after tests */
    udp->next = DATA;
    return;
}

void split_udp(Frame *frame, unsigned char *buf) {
    Header *udp;
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    udp = &frame->headers[frame->header_index];
    udp->name = UDP;
    udp->size = UDP_HEADER_SIZE;
    copy_udp_header_data(udp, buf + frame->offset);
    copy_udp_next_header(udp);
    frame->offset += (unsigned int) udp->size;
    return;
}
