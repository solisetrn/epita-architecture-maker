CC = gcc
CFLAGS = -Wall -Wextra -g
DEBUG = -fsanitize=address
FILES = main.o tree_manager.o parser.o dlist_manager.o executer.o
RM = rm -f

all: ${FILES}
	${CC} ${FILES} -o ArchiMaker ${CFLAGS} ${DEBUG}

reset:
	rm -rf tests/
	mkdir tests/
	clear

clean:
	${RM} *.o ArchiMaker
