#include <semaphore.h>
#include "barrier.h"
#include <stdio.h>
int barrier_init(struct barrier* b,int no)
{

  if(no<0)
  {
      printf("Negative no given for threads!!");
      return  -1;
  }

  b->n=no;
  b->count=0;

  int status=sem_init(&(b->mutex),0,1);
  if(status!=0)
  {
    perror("error!");
    return status;
  }

  status=sem_init(&(b->turnstile),0,0);
  if(status!=0)
  {
    perror("error!");
    return status;
  }

return 0;
}


int barrier_point(struct barrier* b)
{

 int status;

 status=sem_wait(&b->mutex);           //to increment count in mutual exclusion
 if(status!=0)
 {
   perror("error!");            //return -1 on any error                
    return status;
 }

 b->count += 1;               //increment count
 if (b->count == b->n)         //if last thread reached then signal all threads
 {
   int i=0;
  while(i < b->n)
  {
    status=sem_post(&b->turnstile);
    if(status!=0)
   {
     perror("error!");
     return status;
   }

    i++;
  }
  b->count=0;                     //for reuseability
 }
 
 status=sem_post(&b->mutex);        //posting the above semaphore(mutex)
 if(status!=0)
 {
    perror("error!");
    return status;
 }

 status=sem_wait(&b->turnstile);   //each thread waits here till the slowest  thread is reached.
 if(status!=0)
 {
   perror("error!");
    return status;
 }

 
 return 0;
}

