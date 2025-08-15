#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>

#define MAX_LOG_MSG 0x40 * sizeof (char)
#define TRACE "TRACE"
#define DEBUG "DEBUG"
#define WARNN "WARNN"
#define ERROR "ERROR"

#define append_log(count, level, date, time, ver, mod, pos, msg)\
    do {\
        fprintf(\
            logfile,\
            "[0x%lx][%-5.5s][%-10.10s][%-15.15s][VER:%-5.6s][MOD:%-.32s][POS:%-.32s][%-.64s]",\
            count, level, date, time, ver, mod, pos, msg\
        );\
    } while (0)

void logger(mqd_t qlog);

#endif
