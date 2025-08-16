#include "core/dispatcher.h"

static Priority priority;
static Tags tags;

static unsigned int get_ethertype(unsigned char *buf) {
   	unsigned int ethertype = (buf[12] << 8) | (buf[13]);
    return ethertype;
}

static unsigned int get_mtu(unsigned int sockfd, unsigned char *iface) {
    signed int mtu = get_mtu_ifname(sockfd, iface);
    if (mtu <= 0) {
        /* log */
        mtu = BACKUP_MTU;
    }
    return (unsigned int) mtu;
}

static void init_sockaddr_ll_structure(struct sockaddr_ll *addr, unsigned char *iface) {
    unsigned int ifindex = if_nametoindex(iface);
    if (ifindex == 0) {
        /* log */
        IFINDEX_ERROR(iface);
    }
    else {
        addr->sll_family = AF_PACKET;
        addr->sll_protocol = htons(ETH_P_ALL);
        addr->sll_ifindex = ifindex;
    }
    return;
}

unsigned int init_socket(unsigned char *iface) {
    signed int status;
    signed int sockfd;
    struct sockaddr_ll addr;
    init_sockaddr_ll_structure(&addr, iface);
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    /* log */
    status += bind(sockfd, (struct sockaddr *) &addr, sizeof (struct sockaddr_ll));
    status += sockfd;
    /* log */
    return (status < sockfd) ? 0 : 1;
}

unsigned int check_queue_size(mqd_t qframe) {
    struct mq_attr attr;
    signed int status = mq_getattr(qframe, &attr);
    if (status == -1) {/* log */ MQ_GETATTR_ERROR(status);}
    return (attr.mq_curmsgs == attr.mq_maxmsg) ? 1 : 0;
}

void start_sniffing(unsigned int sockfd, unsigned char *iface, mqd_t qframe, unsigned short *runstat) {
    signed int length, mtu = get_mtu(sockfd, iface);
    while (*runstat) {
        unsigned char tmp_rcv_buf[mtu + 14];
        length = recvfrom(sockfd, tmp_rcv_buf, sizeof (tmp_rcv_buf), MSG_TRUNC, NULL, 0);
        /* log */
        if (check_queue_size(qframe) == 1) {
            unsigned int ethertype = get_ethertype(tmp_rcv_buf);
            signed int mq_send_status;
            switch (ethertype) {
                case (IPv4) :
                    priority = IPv4_PRIO;
                    /* log */
                    break;
                case (IPv6) :
                    priority = IPv6_PRIO;
                    /* log */
                    break;
                case (ARP) :
                    priority = ARP_PRIO;
                    /* log */
                    break;
                default :
                    /* log */
                    break;
            }
            mq_send_status = mq_send(qframe, tmp_rcv_buf, sizeof (tmp_rcv_buf), priority);
            /* log */
        }
        /* log */
    }
}

void dispatcher(mqd_t qframe, unsigned char *iface, pthread_mutex_t *lock, unsigned short *runstat, unsigned int *sockfd) {
    /* log */
    *sockfd = init_socket(iface);
    if (!*sockfd) {terminate(lock, runstat, "Unable to create the socket : shutting down..."); return;}
    start_sniffing(*sockfd, iface, qframe, runstat);
    /* log */
    return;
}
