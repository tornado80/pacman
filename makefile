all : main

main : 
	gcc -o main ./main.c ./utils/terminal.c ./utils/gui.c ./game/core.c ./game/ai.c ./ds/linkedlist.c ./ds/queue.c ./ds/stack.c

clean :
	rm main