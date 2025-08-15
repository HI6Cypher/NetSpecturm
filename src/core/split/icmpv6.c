#include "core/split/icmpv6.h"

static Tags tags;

static void copy_icmpv6_next(Header *icmp, unsigned char *buf) {
    /* implementation of this function will be after tests */
    icmp->next = DATA;
    icmp->size = 8 * sizeof (char);
    return;
}

static void copy_icmpv6_header_data(Header *icmp, unsigned char *buf) {
    copy_header_data(icmp, buf);
    return;
}

void split_icmpv6(Frame *frame, unsigned char *buf) {
    Header *icmp;
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    icmp = frame->headers[frame->header_index];
    icmp->name = ICMPv6;
    copy_icmpv6_next(icmp, buf + frame->offset);
    copy_icmpv6_header_data(icmp);
    frame->offset += (unsigned int) icmp->size;
    return;
}
