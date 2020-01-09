all : main

main : linkedlist.o gui.o terminal.o game.o core.o 
	gcc -o main terminal.o game.o gui.o core.o linkedlist.o

gui.o : gui.c
	gcc -c gui.c

core.o : core.c
	gcc -c core.c

linkedlist.o : linkedlist.c
	gcc -c linkedlist.c

terminal.o : terminal.c
	gcc -c terminal.c

game.o : game.c
	gcc -c game.c

clean :
	rm *.o main