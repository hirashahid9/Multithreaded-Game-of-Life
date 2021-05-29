/*
 * The Game of Life
 *
 * a cell is born, if it has exactly three neighbours 
 * a cell dies of loneliness, if it has less than two neighbours 
 * a cell dies of overcrowding, if it has more than three neighbours 
 * a cell survives to the next generation, if it does not die of loneliness 
 * or overcrowding 
 *
 * In my version, a 2D array of ints is used.  A 1 cell is on, a 0 cell is off.
 * The game plays 100 rounds, printing to the screen each time.  'x' printed
 * means on, space means 0.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>
#include "barrier.h"

/* dimensions of the screen */

#define BOARD_WIDTH	79
#define BOARD_HEIGHT	24

/* set everthing to zero */

struct barrier bar;
int board[BOARD_WIDTH][BOARD_HEIGHT];

void initialize_board () {
	int	i, j;

	for (i=0; i<BOARD_WIDTH; i++) 
        {
          for (j=0; j<BOARD_HEIGHT; j++) 
	  {
        	board[i][j] = rand()%2;
           }
         }
}

/* add to a width index, wrapping around like a cylinder */

int xadd (int i, int a) {
	i += a;
	while (i < 0) i += BOARD_WIDTH;
	while (i >= BOARD_WIDTH) i -= BOARD_WIDTH;
	return i;
}

/* add to a height index, wrapping around */

int yadd (int i, int a) {
	i += a;
	while (i < 0) i += BOARD_HEIGHT;
	while (i >= BOARD_HEIGHT) i
 -= BOARD_HEIGHT;
	return i;
}

/* return the number of on cells adjacent to the i,j cell */

int adjacent_to (int board[][BOARD_HEIGHT], int i, int j) {
	int	k, l, count;

	count = 0;

	/* go around the cell */

	for (k=-1; k<=1; k++) for (l=-1; l<=1; l++)

		/* only count if at least one of k,l isn't zero */

		if (k || l)
			if (board[xadd(i,k)][yadd(j,l)]) count++;
	return count;
}

void* play (void* param) {
/*
	(copied this from some web page, hence the English spellings...)

	1.STASIS : If, for a given cell, the number of on neighbours is 
	exactly two, the cell maintains its status quo into the next 
	generation. If the cell is on, it stays on, if it is off, it stays off.

	2.GROWTH : If the number of on neighbours is exactly three, the cell 
	will be on in the next generation. This is regardless of the cell's 		current state.

	3.DEATH : If the number of on neighbours is 0, 1, 4-8, the cell will 
	be off in the next generation.
*/
	int	i, j, a, newboard[BOARD_WIDTH];
        int index=(int) param;
       
	/* for each cell, apply the rules of Life */
        //each thread will store the new values of row assigned to it in  1D array newboard.

	for (i=0; i<BOARD_WIDTH; i++) 
        {
		a = adjacent_to (board, i, index);
		if (a == 2) newboard[i] = board[i][index];
		if (a == 3) newboard[i] = 1;
		if (a < 2) newboard[i] = 0;
		if (a > 3) newboard[i] = 0;
	}

	/* copy the new board back into the old board */
        
      //wait till the slowest thread stores new values of row assigned
        int status=barrier_point(&bar);
        if(status!=0)
        { 
          printf("Error!!");
          return 0;
        }
	
      //each thread copies its newboard[] contents into the respective row in board.
           for (j=0; j<BOARD_WIDTH; j++) 
           {
		board[j][index] = newboard[j];
	   }

  return (void*)NULL;

}

/* print the life board */

void print (int board[][BOARD_HEIGHT]) {
	int	i, j;

	/* for each row */

	for (j=0; j<BOARD_HEIGHT; j++) {

		/* print each column position... */

		for (i=0; i<BOARD_WIDTH; i++) {
			printf ("%c", board[i][j] ? 'x' : ' ');
                   
		}

		/* followed by a carriage return */

		printf ("\n");
	}
}


/* main program */

int main () 
{
        int i, j;
        pthread_t tid[BOARD_HEIGHT];
	initialize_board ();


	/* play game of life 100 times */

       int status=barrier_init(&bar,BOARD_HEIGHT);
        if(status==-1)
        {
           printf("error");
           return 0;
        }

	for (i=0; i<100; i++) 
       {
		print (board);
                sleep(1);
       
	       for (j=0;j<BOARD_HEIGHT;j++)        //create thread for each row
	       {
		  pthread_create(&tid[j],NULL,&play,(void*)j);
	       }                                   
    //pass index assigned to thread as parameter for the function play
	
	
	      for (j=0;j<BOARD_HEIGHT ; j++) 
	       {
		  pthread_join(tid[j], NULL);
	       }
                
		/* clear the screen using VT100 escape codes */

		puts ("\033[H\033[J");
	}
}
