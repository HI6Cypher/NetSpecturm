#include "core/split/icmpv4.h"

static Tags tags;

static void copy_icmpv4_next(Header *icmp, unsigned char *buf) {
    unsigned int type = buf[0];
    switch (type) {
        case (0) :
        case (8) :
        case (10) :
        case (15) :
        case (16) :
            icmp->next = DATA;
            icmp->size = 8 * sizeof (char);
            break;
        case (3) :
        case (4) :
        case (5) :
        case (11) :
        case (12) :
            icmp->next = IPv4;
            icmp->size = 8 * sizeof (char);
            break;
        case (9) :
            icmp->next = DATA;
            icmp->size = (8 + buf[4] * buf[5] * 4) * sizeof (char);
            break;
        case (13) :
        case (14) :
            icmp->next = DATA;
            icmp->size = 20 * sizeof (char);
            break;
        case default :
            icmp->next = DATA;
            icmp->size = 8 * sizeof (char);
            break;
    }
    return;
}

static void copy_icmpv4_header_data(Header *icmp, unsigned char *buf) {
    icmp->data = (unsigned char *) malloc(icmp->size);
    if (!icmp->data) {/* log */ return;}
    memcpy(icmp->data, buf, icmp->size);
    return;
}

void split_icmpv4(Frame *frame, unsigned char *buf) {
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    frame->headers[frame->header_index]->name = ICMPv4;
    copy_icmpv4_next(frame->headers[frame->header_index], buf + frame->offset);
    copy_icmpv4_header_data(frame->headers[frame->header_index], buf + frame->offset);
    frame->offset += (unsigned int) ICMP_HEADER_SIZE;
    return;
}
