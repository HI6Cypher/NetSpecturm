#include "tools/get_mtu.h"

unsigned int get_mtu_size(unsigned int sockfd, unsigned char *iface) {
    struct ifreq ifr;
    signed int status;
    memcpy(ifr.ifr_name, iface, strlen(iface) + 1);
    status = ioctl(sockfd, SIOCGIFMTU, &ifr);
    return (status == -1) ? 0 : ifr.ifr_mtu;
}
