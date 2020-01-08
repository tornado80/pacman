all : main

main : terminal.o game.o gui.o
	gcc -o main terminal.o game.o gui.o

gui.o : gui.c
	gcc -c gui.c

terminal.o : terminal.c
	gcc -c terminal.c

game.o : game.c
	gcc -c game.c

clean :
	rm *.o main