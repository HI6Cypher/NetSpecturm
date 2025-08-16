#include "logger/logger.h"

static FILE logfile;

unsigned int create_file(FILE *logfile) {
    unsigned char filename[256];
    sprintf(filename, "logfile_%ld.log", (long) time(NULL));
    logfile = fopen(filename, "w");
    if (!logfile) {
        switch (errno) {
            case (EACCES) :
                fprintf(stderr, "Access Denied : %s %s\n\n", strerror(errno), filename);
                break;
            case (ENOSPC) :
                fprintf(stderr, "Insufficient Storage : %s %s\n\n", strerror(errno), filename);
                break;
            default :
                fprintf(stderr, "Error : %s %s\n\n", strerror(errno), filename);
                break;
        }
        return 0;
    }
    return 1;
}

void logger(mqd_t qlog, pthread_mutex_t *lock, unsigned short *runstat) {
    unsigned int file_status = create_file(&logfile);
    ssize_t length;
    unsigned long count = 1;
    if (!file_status) {shutdown(lock, runstat, "Unable to create file : shutting down..."); return;}
    while (*runstat) {
        Log tmp_log;
        unsigned char date[11];
        unsigned char clock[16];
        length = mq_receive(qlog, (unsigned char *) &tmp_log, sizeof (tmp_log), NULL);
        if (!length) {shutdown(lock, runstat, "Error on mqueue : shutting down..."); return;}
        strftime(date, sizeof (date), "%Y-%m-%d", &tmp_log.time);
        strftime(clock, sizeof (clock), "%H:%M:%S.%f", &tmp_log.time);
        append_log(count, tmp_log.level, date, clock, tmp_log.ver, tmp_log.mod, tmp_log.pos, tmp_log.msg);
    }
    return;
}
