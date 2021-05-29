my_prog : barrier.o game_of_life.o
	gcc -o my_prog barrier.o game_of_life.o -lpthread

barrier.o : barrier.c barrier.h
	gcc -c barrier.c -lpthread -w

game_of_life.o : game_of_life.c barrier.h
	gcc -c game_of_life.c -lpthread -w

