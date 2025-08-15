#include "core/split/arp.h"

static Tags tags;

static void copy_arp_header_data(Header *arp, unsigned char *buf) {
    arp->data = (unsigned char *) malloc(arp->size);
    if (!arp->data) {/* log */ return;}
    memcpy(arp->data, buf, arp->size);
    return;
}

void split_arp(Frame *frame, unsigned char *buf) {
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    Header *arp = &frame->headers[frame->header_index];
    arp->name = ARP;
    arp->next = DATA;
    arp->size = ARPIP_HEADER_SIZE;
    copy_arp_header_data(arp, buf + frame->offset);
    frame->offset += (unsigned int) ARPIP_HEADER_SIZE;
    return;
}
