#include "core/split/igmp.h"

static Tags tags;

static void copy_igmp_header_data(Header *igmp, unsigned char *buf) {
    copy_header_data(igmp, buf);
    return;
}

static size_t get_igmp_memship_query_size(unsigned char *buf) {
    unsigned int number_of_sources = (
        (buf[10] << 8) |
        (buf[11] & 0xff)
    );
    return (8 + number_of_sources * 4) * sizeof(char);
}

static size_t get_igmpv3_memship_report_size(unsigned char *buf) {
    unsigned int record;
    unsigned int records_size;
    unsigned int offset = 8;
    unsigned int number_of_group_records = (
        (buf[6] << 8) |
        (buf[7] & 0xff)
    );

    for (record = 0; record < number_of_group_records; record++) {
        unsigned int number_of_sources = (
            ((buf + offset)[2] << 8) |
            ((buf + offset)[3] & 0xff)
        );
        unsigned int tmp_record_size = 8 + (buf + offset)[1] * 4 + number_of_sources * 4;
        records_size += tmp_record_size;
        offset += tmp_record_size;
    }
    return (8 + records_size) * sizeof (char);
}

void split_igmpv1_memship_report(Frame *frame, unsigned char *buf) {
    Header *igmp = &frame->headers[frame->header_index];
    igmp->size = 8;
    igmp->next = DATA;
    copy_igmp_header_data(igmp, buf + frame->offset);
    frame->offset += (unsigned int) igmp->size;
    return;
}

void split_igmpv2_memship_report(Frame *frame, unsigned char *buf) {
    split_igmpv1_memship_report(frame, buf);
    return;
}

void split_igmpv2_leave_group(Frame *frame, unsigned char *buf) {
    split_igmpv1_memship_report(frame, buf);
    return;
}

void split_igmp_memship_query(Frame *frame, unsigned char *buf) {
    Header *igmp = &frame->headers[frame->header_index];
    igmp->size = get_igmp_memship_query_size(buf + frame->offset);
    igmp->next = DATA;
    copy_igmp_header_data(igmp, buf + frame->offset);
    frame->offset += (unsigned int) igmp->size;
    return;
}

void split_igmpv3_memship_report(Frame *frame, unsigned char *buf) {
    Header *igmp = &frame->headers[frame->header_index];
    igmp->size = get_igmpv3_memship_report_size(buf + frame->offset);
    igmp->next = DATA;
    copy_igmp_header_data(igmp, buf + frame->offset);
    frame->offset += (unsigned int) igmp->size;
    return;
}

void split_igmp_any(Frame *frame, unsigned char *buf) {
    split_igmpv1_memship_report(frame, buf);
    return;
}

void split_igmp(Frame *frame, unsigned char *buf) {
    unsigned short type = (buf + frame->offset)[0] & 0xf;
    if (!new_header(frame)) {/* log */ frame->status = 1; return;}
    frame->headers[frame->header_index].name = IGMP;
    switch (type) {
        case (0x12) :
            split_igmpv1_memship_report(frame, buf);
            break;
        case (0x16) :
            split_igmpv2_memship_report(frame, buf);
            break;
        case (0x17) :
            split_igmpv2_leave_group(frame, buf);
            break;
        case (0x11) :
            split_igmp_memship_query(frame, buf);
            break;
        case (0x22) :
            split_igmpv3_memship_report(frame, buf);
            break;
        default :
            split_igmp_any(frame, buf);
    }
    return;
}
