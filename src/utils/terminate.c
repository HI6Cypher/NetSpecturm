#include "utils/terminate.h"

void terminate(pthread_mutex_t *lock, unsigned short *runstat, unsigned char *msg) {
    fflush(stdout);
    fprintf(stderr, "%s\n", msg);
    pthread_mutex_lock(lock);
    *runstat = 0;
    pthread_mutex_unlock(lock);
    return;
}
