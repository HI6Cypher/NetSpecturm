#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include <pthread.h>
#include <stdio.h>

void terminate(pthread_mutex_t *lock, unsigned short *runstat, unsigned char *msg);

#endif
