#ifndef LOG_TOOL_H
#define LOG_TOOL_H

#include "netrum.h"
#include "logger/logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <mqueue.h>

void LOG(
    const unsigned char *level,
    const unsigned char *mod,
    const unsigned char *pos,
    unsigned char *msg,
    ...
);

#endif
