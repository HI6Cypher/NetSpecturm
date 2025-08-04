#include "core/dispatcher.h"

static Priority priority;
static Ethertype ethertype;

static unsigned int get_ethertype(unsigned char *buf) {
    unsigned int ethertype = (buf[12] << 8) | (buf[13]);
    return ethertype;
}

unsigned int init_socket(unsigned char *iface) {
    signed int status;
    unsigned int sockfd;
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    /* log */
    status += setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, iface, strlen(iface));
    status += sockfd;
    /* log */
    return (status < sockfd) ? 0 : 1;
}

unsigned int check_queue_size(mqd_t mqdes) {
    struct mq_attr attr;
    signed int status = mq_getattr(mqdes, &attr);
    if (status == -1) {/* log */ MQ_GETATTR_ERROR(status);}
    return (attr.mq_curmsgs == attr.mq_maxmsg) ? 1 : 0;
}

void start_sniffing(unsigned int sockfd, unsigned char *iface, mqd_t mqdes) {
    signed int length;
    signed int mtu_size = get_mtu_size(sockfd, iface);
    /* log */
    if (mtu_size <= 0) {/* log */ MTU_ERROR(mtu_size);}
    while (1) {
        unsigned char tmp_rcv_buf[mtu_size];
        length = recvfrom(sockfd, tmp_rcv_buf, sizeof (tmp_rcv_buf), 0, NULL, 0);
        /* log */
        if (check_queue_size(mqdes) == 1) {
            unsigned int type = get_ethertype(tmp_rcv_buf);
            signed int mq_send_status;
            switch (type) {
                case (IPv4) :
                    priority = IPv4_PRIO;
                    /* log */
                case (IPv6) :
                    priority = IPv6_PRIO;
                    /* log */
                case (ARP) :
                    priority = ARP_PRIO;
                    /* log */
                default :
                    UNSUPPORTED_ETHERTYPE(type);
                    /* log */
            }
            mq_send_status = mq_send(mqdes, tmp_rcv_buf, sizeof (tmp_rcv_buf), priority);
            if (mq_send_status == -1) {/* log */ MQ_SEND_ERROR(mq_send_status);}
        }
        else {
            /* log */
            QUEUE_SIZE_ERROR(length);
        }
    }
}
