#include "core/consumer.h"

static Tags tag;

static void prepare_frame(Frame *frame, unsigned int seq, unsigned int ifindex, size_t size) {
    frame->status = 0;
    frame->offset = 14;
    frame->header_index = 0;
    frame->header_count = 1;
    frame->sequence = seq;
    frame->ifindex = ifindex;
    frame->time = time(NULL);
    frame->size = size;
    frame->headers = NULL;
    return;
}

static unsigned int get_ethertype(Header *eth) {
    return (eth->data[12] << 8) | (eth->data[13]);
}

static unsigned int split_ethernet(Frame *frame, unsigned char *buf) {
    if (!halloc(frame)) {/* log */ return 0;}
    Header *eth = frame->headers[0];
    eth->name = ETH;
    eth->size = ETH_HEADER_SIZE;
    memcpy(eth->data, buf, eth->size);
    eth->next = get_ethertype(eth);
    return 1;
}

static void split_frame(Frame *frame, unsigned char *buf) {
    switch (frame->headers[frame->header_index]->next) {
        case (ARP) :
            split_arp(frame, buf);
            break;
        case (IPv4) :
            split_ipv4(frame, buf);
            break;
        case (IPv6) :
            split_ipv6(frame, buf);
            break;
        case (ICMPv4) :
            split_icmpv4(frame, buf);
            break;
        case (IGMP) :
            split_igmp(frame, buf);
            break;
        case (IPIP) :
            split_ipip(frame, buf);
            break;
        case (TCP) :
            split_tcp(frame, buf);
            break;
        case (UDP) :
            split_udp(frame,buf);
            break;
        case (IPv6Encap) :
            split_ipv6(frame, buf);
            break;
        case (ICMPv6) :
            split_icmpv6(frame, buf);
            break;
        case (SCTP) :
            split_sctp(frame, buf);
            break;
        case (UDPLite) :
            split_udplite(frame, buf);
            break;
        case (DATA) :
            split_unusable(frame, buf);
            break;
        case default :
            frame->status = 1;
            break;
    }
    return;
}

void splitter(Frame *frame, unsigned char *buf) {
    unsigned short num;
    if (!split_ethernet(frame, buf)) {/* log */ return;}
    while ((!frame->status) & (num <= 8)) {
        split_frame(frame, buf);
        num++;
    }
    return;
}

void handle_output(Frame *frame) {

}

void consumer(mqd_t mqdes, signed int sockfd, unsigned int ifindex) {
    unsigned long seq;
    size_t length;
    signed int mtu_size = get_mtu_ifindex(sockfd, ifindex);
    /* log */
    if (mtu_size <= 0) {/* log */ return;}
    while (runstat) {
        Frame frame;
        unsigned char tmp_buf[mtu_size + 14];
        length = mq_receive(mqdes, tmp_buf, sizeof (tmp_buf), NULL);
        prepare_frame(&frame, seq, ifindex, length);
        splitter(&frame, tmp_buf);
        handle_output(&frame);
        hfree(&frame);
    }
    return;
}
