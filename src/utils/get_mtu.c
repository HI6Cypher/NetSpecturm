#include "tools/get_mtu.h"

unsigned int get_mtu_ifname(unsigned int sockfd, unsigned char *iface) {
    struct ifreq ifr;
    signed int status;
    memcpy(ifr.ifr_name, iface, strlen(iface) + 1);
    status = ioctl(sockfd, SIOCGIFMTU, &ifr);
    return (status == -1) ? status : ifr.ifr_mtu;
}

unsigned int get_mtu_ifindex(unsigned int sockfd, unsigned int ifindex) {
    struct ifreq ifr;
    signed int status;
    unsigned char ifname[IF_NAMESIZE];
    if (if_indextoname(ifindex, ifname) == 0) {status = -1;}
    memcpy(ifr.ifr_name, ifname, strlen(ifname) + 1);
    status = ioctl(sockfd, SIOCGIFMTU, &ifr);
    return (status < 0) ? status : ifr.ifr_mtu;
}
