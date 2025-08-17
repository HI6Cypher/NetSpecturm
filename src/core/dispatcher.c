#include "core/dispatcher.h"

static Priority priority;
static Tags tags;
static pthread_mutex_t *_lock;
static unsigned short *_runstat;

static unsigned int get_ethertype(unsigned char *buf) {
   	unsigned int ethertype = (buf[12] << 8) | (buf[13]);
    return ethertype;
}

static unsigned int get_mtu(unsigned int sockfd, unsigned char *iface) {
    signed int mtu = get_mtu_ifname(sockfd, iface);
    if (mtu <= 0) {
        LOG("ERROR", "dispatcher.c", "get_mtu()", "Unable to fetch mtu, using BACKUP_MTU with value %d", BACKUP_MTU);
        mtu = BACKUP_MTU;
    }
    return (unsigned int) mtu;
}

static void init_sockaddr_ll_structure(struct sockaddr_ll *addr, unsigned char *iface) {
    unsigned int ifindex = if_nametoindex(iface);
    if (ifindex == 0) {
        LOG("ERROR", "dispatcher.c", "init_sockaddr_ll_structure()", "Unable to fetch ifindex from if_nametoindex(), preparing to exit");
        terminate(_lock, _runstat, strerror(errno));
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
    if (sockfd != -1)
        LOG("TRACE", "dispatcher.c", "init_socket()", "socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL) created successfully");
    else
        LOG("ERROR", "dispatcher.c", "init_socket()", "Unable to create socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)");
    status += bind(sockfd, (struct sockaddr *) &addr, sizeof (struct sockaddr_ll));
    status += sockfd;
    if (status < sockfd)
        LOG("ERROR", "dispatcher.c", "init_socket()", "Unable to bind on socket %s", strerror(errno));
    return (status < sockfd) ? 0 : 1;
}

unsigned int check_queue_size(mqd_t qframe) {
    struct mq_attr attr;
    signed int status = mq_getattr(qframe, &attr);
    if (status == -1)
        LOG("ERROR", "dispatcher.c", "check_queue_size()", "function mq_getattr(qframe, &attr) failed");
    return (attr.mq_curmsgs == attr.mq_maxmsg) ? 1 : 0;
}

void start_sniffing(unsigned int sockfd, unsigned char *iface, mqd_t qframe) {
    signed int length, mtu = get_mtu(sockfd, iface);
    while (*_runstat) {
        unsigned char tmp_rcv_buf[mtu + 14];
        length = recvfrom(sockfd, tmp_rcv_buf, sizeof (tmp_rcv_buf), MSG_TRUNC, NULL, 0);
        if (length == -1) {
            LOG("ERROR", "dispatcher.c", "start_sniffing()", "recvfrom() return -1");
            terminate(_lock, _runstat, strerror(errno));
        }
        if (check_queue_size(qframe) == 1) {
            unsigned int ethertype = get_ethertype(tmp_rcv_buf);
            signed int mq_send_status;
            switch (ethertype) {
                case (IPv4) :
                    priority = IPv4_PRIO;
                    break;
                case (IPv6) :
                    priority = IPv6_PRIO;
                    break;
                case (ARP) :
                    priority = ARP_PRIO;
                    break;
                default :
                    LOG("WARNN", "dispatcher.c", "start_sniffing()", "unknown ethertype %d", ethertype);
                    break;
            }
            mq_send_status = mq_send(qframe, tmp_rcv_buf, sizeof (tmp_rcv_buf), priority);
            if (mq_send_status == -1)
                LOG("ERROR", "dispatcher.c", "start_sniffing()", "Unable to send buffer to qframe, mq_send return -1");
        }
    }
}

void dispatcher(mqd_t qframe, unsigned char *iface, pthread_mutex_t *lock, unsigned short *runstat, unsigned int *sockfd) {
    _runstat = runstat;
    _lock = lock;
    LOG("TRACE", "dispatcher.c", "dispatcher()", "dispatcher has been started");
    *sockfd = init_socket(iface);
    if (!*sockfd) {terminate(lock, runstat, "Unable to create the socket : shutting down..."); return;}
    start_sniffing(*sockfd, iface, qframe);
    LOG("WARNN", "dispatcher.c", "dispatcher()", "dispatcher has been closed");
    return;
}
