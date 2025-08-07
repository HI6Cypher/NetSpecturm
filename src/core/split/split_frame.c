#include "core/split/split_frame.h"

static Tags tags;

static unsigned int get_ethertype(Frame *frame) {
    return (
        (frame->link.header_data[12] << 8) |
        (frame->link.header_data[13] & 0xff)
    );
}

static void copy_header_size(Frame *frame) {
    frame->link.header_size = ETHERNET_HEADER_SIZE * sizeof (char);
    return;
}

static void copy_header_data(Frame *frame, unsigned char *buf) {
    frame->link.header_data = malloc(frame->link.header_size);
    if (frame->link.header_data == NULL) {/* log */ MALLOC_ERROR(NULL); return;}
    memcpy(frame->link.header_data, buf, frame->link.header_size);
    return;
}

static void copy_proto_tags(Frame *frame) {
    frame->link.proto_tag = ETH;
    frame->link.next_proto_tag = (Tags) get_ethertype(Frame *frame);
    return;
}

void split_frame(Frame *frame, unsigned char *buf) {
    copy_header_size(frame);
    copy_header_data(frame, buf);
    copy_proto_tags(frame);
    split_network(Frame, unsigned char *buf);
    return;
}
