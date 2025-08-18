#include "core/consumer.h"

static Tags tag;
static Ports ports;

static void prepare_frame(Frame *frame, unsigned int seq, unsigned char *iface, size_t size) {
    frame->offset = 14;
    frame->sequence = seq;
    frame->time = time(NULL);
    frame->size = size;
    frame->headers = NULL;
    memcpy(frame->iface, iface, sizeof (frame->iface));
    return;
}

static unsigned int get_ethertype(Header *eth) {
    return (eth->data[12] << 8) | (eth->data[13]);
}

static unsigned int get_mtu(unsigned int sockfd, unsigned char *iface) {
    signed int mtu = get_mtu_ifname(sockfd, iface);
    if (mtu <= 0) {
        LOG("ERROR", "consumer.c", "get_mtu()", "Unable to fetch mtu, using BACKUP_MTU with value %d", BACKUP_MTU);
        mtu = BACKUP_MTU;
    }
    return (unsigned int) mtu;
}

static void split_ethernet(Frame *frame, unsigned char *buf) {
    Header *eth = frame->headers[0];
    if (!new_header(frame)) {
        LOG("WARNN", "consumer.c", "split_ethernet()", "Unable to allocate new header for ethernet");
        frame->status = 1;
        frame->error = 1;
        return;
    }
    eth->name = ETH;
    eth->size = ETH_HEADER_SIZE;
    copy_header_data(eth, buf + frame->offset);
    eth->next = get_ethertype(eth);
    return;
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
            split_ipv4(frame, buf);
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
        case (UDPLite) :
            split_udp(frame, buf);
            break;
        case (DATA) :
        default :
            split_unusable(frame, buf);
            frame->status = 1;
            break;
    }
    return;
}

void splitter(Frame *frame, unsigned char *buf) {
    unsigned short num;
    split_ethernet(frame, buf);
    while ((!frame->status) & (num <= 8)) {
        split_frame(frame, buf);
        num++;
    }
    if (frame->status) {split_unusable(frame, buf);}
    if (num <= 8)
        LOG("WARNN", "consumer.c", "splitter()", "The frame(seq=%d) with more than eight headers is going to be discarded", frame->sequence);
    return;
}

void handle_output(Frame *frame) {

}

void consumer(mqd_t qframe, signed int sockfd, unsigned char *iface) {
    unsigned long seq;
    ssize_t length;
    signed int mtu = get_mtu(sockfd, iface);
    LOG("TRACE", "consumer.c", "consumer()", "start consumer with mtu %d", mtu);
    while (runstat) {
        Frame frame;
        unsigned char tmp_buf[mtu + 14];
        length = mq_receive(qframe, tmp_buf, sizeof (tmp_buf), NULL);
        prepare_frame(&frame, seq, iface, length);
        splitter(&frame, tmp_buf);
        handle_output(&frame);
        hfree(&frame);
        seq++;
    }
    return;
}
