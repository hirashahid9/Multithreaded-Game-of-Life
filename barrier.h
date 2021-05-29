#include <semaphore.h>

#ifndef _BARRIER_H
#define _BARRIER_H
struct barrier
{

int n;
int count;
sem_t mutex;
sem_t turnstile;


};

int barrier_init(struct barrier* b,int no);
int barrier_point(struct barrier* b);

#endif
