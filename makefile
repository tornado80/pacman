all : main.out

main.out : terminal.o PacManGame.o
	gcc -o main.out terminal.o PacManGame.o

terminal.o : terminal.c
	gcc -c terminal.c

PacManGame.o : PacManGame.c
	gcc -c PacManGame.c

clean :
	rm *.o main.out