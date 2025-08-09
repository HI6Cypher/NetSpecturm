#include "netrum.h"

unsigned int halloc(Frame *frame) {
    Header *tmp = (Header *) realloc(frame->headers, frame->header_count * sizeof (Header));
    if (!tmp) {/* log */ return 0;}
    frame->headers = tmp;
    return 1;
}

void hfree(Frame *frame) {
    unsigned int i;
    for (i = 0; i < frame->header_count; i++)
        free(frame->headers[i]->data);
    free(frame->headers);
    return;
}

unsigned int new_header(Frame *frame, size_t header_size) {
    if (!halloc(frame)) {/* log */ return 0;}
    frame->header_index++;
    frame->header_count++;
}
