#include "utils/log_tool.h"

void LOG(
    const unsigned char *level,
    const unsigned char *mod,
    const unsigned char *pos,
    unsigned char *msg,
    ...
) {
    va_list args;
    Log new_log;
    signed int stat;
    time_t timep = time(NULL);
    unsigned char formatted_msg[MAX_LOG_MSG];
    unsigned char *fmt = msg;
    memcpy(new_log.level, level, sizeof (new_log.level));
    memcpy(new_log.ver, version, sizeof (new_log.ver));
    memcpy(new_log.mod, mod, sizeof (new_log.mod));
    memcpy(new_log.pos, pos, sizeof (new_log.pos));
    va_start(args, msg);
    while (*fmt++) {
        if (*fmt == '%') {
            unsigned short found;
            switch (*(fmt + 1)) {
                case ('s') :
                    snprintf(formatted_msg, sizeof (formatted_msg), msg, va_arg(args, unsigned char *));
                    found = 1;
                    break;
                case ('d') :
                    snprintf(formatted_msg, sizeof (formatted_msg), msg, va_arg(args, unsigned int));
                    found = 1;
                    break;
            }
            if (found)
                break;
        }
    }
    va_end(args);
    memcpy(new_log.msg, formatted_msg, sizeof (new_log.msg));
    new_log.time = localtime(&timep);
    stat = mq_send(qlog, (unsigned char *) &new_log, sizeof (new_log), 0);
    if (!stat)
        fprintf(stderr, "Unable to log(%-5.5s, %-.32s, %-.32s, %-.64s)", level, mod, pos, msg);
    return;

}
