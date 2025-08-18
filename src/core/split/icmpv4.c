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
        default :
            icmp->next = DATA;
            icmp->size = 8 * sizeof (char);
            break;
    }
    return;
}

void split_icmpv4(Frame *frame, unsigned char *buf) {
    Header *icmp;
    if (!new_header(frame)) {
        LOG("WARNN", "icmpv4.c", "split_icmpv4()", "Unable to allocate new header for ICMPv4 with offset %d", frame->offset);
        frame->status = 1;
        frame->error = 1;
        return;
    }
    icmp = &frame->headers[frame->header_index];
    icmp->name = ICMPv4;
    copy_icmpv4_next(icmp, buf + frame->offset);
    copy_icmpv4_header_data(icmp, buf + frame->offset);
    frame->offset += icmp->size;
    return;
}
