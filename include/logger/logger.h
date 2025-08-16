#ifndef LOGGER_H
#define LOGGER_H

#include "utils/terminate.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <errno.h>

#define MAX_LOG_MSG 0x40 * sizeof (char)
#define MAX_LOG_LEVEL 0x6 * sizeof (char)
#define MAX_LOG_VER 0x7 * sizeof (char)
#define MAX_LOG_MOD 0x20 * sizeof (char)
#define MAX_LOG_POS 0x20 * sizeof (char)
#define TRACE "TRACE"
#define DEBUG "DEBUG"
#define WARNN "WARNN"
#define ERROR "ERROR"

#define append_log(count, level, date, clock, ver, mod, pos, msg)\
    do {\
        fprintf(\
            &logfile,\
            "[0x%lx][%-5.5s][%-10.10s][%-15.15s][VER:%-5.6s][MOD:%-.32s][POS:%-.32s][%-.64s]",\
            count, level, date, clock, ver, mod, pos, msg\
        );\
    } while (0)

typedef struct {
    unsigned char level[MAX_LOG_LEVEL];
    unsigned char ver[MAX_LOG_VER];
    unsigned char mod[MAX_LOG_MOD];
    unsigned char pos[MAX_LOG_POS];
    unsigned char msg[MAX_LOG_MSG];
    struct tm time;
} Log;

unsigned int create_logfile(FILE *logfile);
void logger(mqd_t qlog, pthread_mutex_t *lock, unsigned short *runstat);

#endif
