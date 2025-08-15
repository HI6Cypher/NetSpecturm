#include "core/split/arp.h"

static Tags tags;

void split_arp(Frame *frame, unsigned char *buf) {
    Header *arp;
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    arp = &frame->headers[frame->header_index];
    arp->name = ARP;
    arp->next = DATA;
    arp->size = ARPIP_HEADER_SIZE;
    copy_arp_header_data(arp, buf + frame->offset);
    frame->offset += (unsigned int) arp->size;
    return;
}
