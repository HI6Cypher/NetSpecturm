#include "core/split/icmpv6.h"

static Tags tags;

static void copy_icmpv6_next(Header *icmp, unsigned char *buf) {
    /* implementation of this function will be after tests */
    icmp->next = DATA;
    icmp->size = 8 * sizeof (char);
    return;
}

void split_icmpv6(Frame *frame, unsigned char *buf) {
    Header *icmp;
    if (!new_header(frame)) {
        LOG("WARNN", "icmpv6.c", "split_icmpv6()", "Unable to allocate new header for ICMPv6 with offset %d", frame->offset);
        frame->status = 1;
        frame->error = 1;
        return;
    }
    icmp = &frame->headers[frame->header_index];
    icmp->name = ICMPv6;
    copy_icmpv6_next(icmp, buf + frame->offset);
    copy_icmpv6_header_data(icmp, buf);
    frame->offset += (unsigned int) icmp->size;
    return;
}
