
# Overview

It is a multithreaded version of Conway's "Game of Life". 

To get an overview of thie game refer to this link: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

The actual cod of the  game as defined in this link: https://www.cs.utexas.edu/users/djimenez/utsa/cs1713-3/c/life.txt.

Barrier functionality is used for the purpose of multithreading as defined here http://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf



# Compilation and Execution

1. Write "make" on command prompt and press enter.

2. Then write "./my_prog" and press enter.

A makefile has been created by me so that we don't need to compile all files every time we execute the code.


# Requirements

Linux Operating System


# Snapshot of Running Program

![image](https://user-images.githubusercontent.com/55246052/120070411-d3c46d80-c0a3-11eb-85a8-56896b7aad1b.png)



# Changes Made
1. I have passed barrier objects pointer in barrier_init and barrier_point as without this, the function would not know which object to access.
2. In barrier_init,the function return -1 whenever sem_init return non zero value(error). 
3. In barrier_point, the function return -1 whenever sem_wait(&sem) or sem_post(&sem) return non zero value(error).
4. Reuseability of barrier is implemented by assigning barrier->count=0 when count==no of threads. And the semaphore values also become same as thier initialization values ath this instance.
5. Barrier point is placed after the threads have stored new values in thier respective 1D arrays and before the threads write new values to the board.
