paint.out: main.o commands.o canvas_manipulation.o user_interaction.o
	gcc -g -Wall -Werror -o paint.out main.o commands.o canvas_manipulation.o user_interaction.o

main.o: main.c
	gcc -g -Wall -Werror -c main.c

commands.o: commands.c commands.h
	gcc -g -Wall -Werror -c commands.c

canvas_manipulation.o: canvas_manipulation.c canvas_manipulation.h
	gcc -g -Wall -Werror -c canvas_manipulation.c

user_interaction.o: user_interaction.c user_interaction.h
	gcc -g -Wall -Werror -c user_interaction.c

clean:
	rm -fr *.o *.out