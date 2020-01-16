all : main

main : linkedlist.o gui.o terminal.o game.o core.o queue.o ai.o
	gcc -o main terminal.o game.o gui.o core.o linkedlist.o queue.o ai.o

gui.o : gui.c
	gcc -c gui.c

queue.o : queue.c
	gcc -c queue.c

core.o : core.c
	gcc -c core.c

linkedlist.o : linkedlist.c
	gcc -c linkedlist.c

terminal.o : terminal.c
	gcc -c terminal.c

game.o : game.c
	gcc -c game.c

ai.o : ai.c
	gcc -c ai.c

clean :
	rm *.o main