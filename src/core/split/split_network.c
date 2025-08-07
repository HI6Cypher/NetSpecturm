#include "core/split/split_network.h"

static Tags tags;

static unsigned int get_ipv4_header_size(Frame *frame, unsigned char *buf) {
    unsigned int index = frame->link.header_size;
    return (
        ((buf + index)[0] & 0xf) << 2
    );
}

static unsigned int check_network_compound(Frame *frame) {
    switch (frame->net.next_proto_tag) {
        case ICMPv4 :
        case ICMPv6 :
        case IGMP :
        case IPIP :
        case IPv6C :
        case default :
            return 0;
    }
    return 1;
}

static void copy_arp_header_size(Frame *frame) {
    frame->net.header_size = ARPIP_HEADER_SIZE * sizeof (char);
    return;
}

static void copy_arp_header_data(Frame *frame, unsigned char *buf) {
    unsigned int index = frame->link.header_size;
    frame->net.header_data = malloc(frame->net.header_size);
    if (frame->net.header_data == NULL) {/* log */ return;}
    memcpy(frame->net.header_data, buf + index, frame->net.header_size);
    return;
}

static void copy_arp_proto_tags(Frame *frame) {
    frame->net.proto_tag = ARP;
    frame->net.next_proto_tag = DATA;
    return;
}

static void copy_ipv4_header_size(Frame *frame, unsigned char *buf) {
    frame->net.header_size = get_ipv4_header_size(frame, buf);
    return;
}

static void copy_ipv4_header_data(Frame *frame, unsigned char *buf) {
    unsigned int index = frame->link.header_size;
    frame->net.header_data = malloc(frame->net.header_size);
    if (frame->net.header_data == NULL) {/* log */ return;}
    memcpy(frame->net.header_data, buf + index, frame->net.header_size);
    return;
}

static void copy_ipv4_proto_tags(Frame *frame) {
    frame->net.proto_tag = IPv4;
    frame->net.next_proto_tag = frame->net.header_data[9];
    return;
}

static void copy_ipv6_header_size(Frame *frame) {
    frame->net.header_size = 40 * sizeof (char);
    return;
}

static void copy_ipv6_header_data(Frame *frame, unsigned char *buf) {
    unsigned int index = frame->link.header_size;
    frame->net.header_data = malloc(frame->net.header_size);
    if (frame->net.header_data == NULL) {/* log */ return;}
    memcpy(frame->net.header_data, buf + index, frame->net.header_size);
    return;
}

static void copy_ipv6_proto_tags(Frame *frame) {
    frame->net.proto_tag = IPv6;
    frame->net.next_proto_tag = frame->net.header_data[7];
    return;
}

void split_network(Frame *frame, unsigned char *buf) {
    switch (frame->link.next_proto_tag) {
        case ARP :
            split_network_arp(frame, buf);
        case IPv4 :
            split_network_ipv4(frame, buf);
        case IPv6 :
            split_network_ipv6(frame, buf);
    }
    return;
}

void split_network_arp(Frame *frame, unsigned char *buf) {
    if (!check_arp_proto_type(buf)) {/* log */ return 0;}
    copy_arp_header_size(frame);
    copy_arp_header_data(frame, buf);
    copy_arp_proto_tags(frame);
    split_unusable_data(frame, buf);
    return;
}

void split_network_ipv4(Frame *frame, unsigned char *buf) {
    copy_ipv4_header_size(frame, buf);
    copy_ipv4_header_data(frame, buf);
    copy_ipv4_proto_tags(frame);
    if (check_network_compound(frame) == 1) {
        /* log */
        move_net_to_comnet(frame);
        split_network_compound(frame, buf);
    }
    else {
        /* log */
        split_transport(frame, buf);
    }
    return;
}

void split_network_ipv6(Frame *frame, unsigned char *buf) {
    copy_ipv6_header_size(frame, buf);
    copy_ipv6_header_data(frame, buf);
    copy_ipv6_proto_tags(frame);
    if (check_network_compound(frame) == 1) {
        /* log */
        move_net_to_comnet(frame);
        split_network_compound(frame, buf);
    }
    else {
        /* log */
        split_transport(frame, buf);
    }
    return;   
}
