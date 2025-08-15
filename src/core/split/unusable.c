#include "core/split/unusable.h"

static Tags tags;

void split_unusable(Frame *frame, unsigned char *buf) {
    Header *data;
    if (!new_header(frame)) {/* log */ return;}
    data = frame->headers[frame->header_index];
    data->name = DATA;
    data->next = NONE;
    data->size = (frame->size - frame->offset) * sizeof (char);
    copy_header_data(data, buf + frame->offset);
    frame->offset += (unsigned int) data->size;
    return;
}
