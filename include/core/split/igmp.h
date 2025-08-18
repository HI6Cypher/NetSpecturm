#ifndef IGMP_H
#define IGMP_H

#include "netrum.h"
#include "utils/hmem.h"
#include "utils/log_tool.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define copy_igmp_header_data(header, buf) copy_header_data(header, buf)

void split_igmpv1_memship_report(Frame *frame, unsigned char *buf);
void split_igmpv2_memship_report(Frame *frame, unsigned char *buf);
void split_igmpv3_memship_report(Frame *frame, unsigned char *buf);
void split_igmpv2_leave_group(Frame *frame, unsigned char *buf);
void split_igmp_memship_query(Frame *frame, unsigned char *buf);
void split_igmp_any(Frame *frame, unsigned char *buf);
void split_igmp(Frame *frame, unsigned char *buf);

#endif
