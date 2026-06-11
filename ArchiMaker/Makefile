CC = gcc
CFLAGS = -Wall -Wextra -g
DEBUG = -fsanitize=address
FILES = main.o tree_manager.o pretty_printer.o
RM = rm -f

all: ${FILES}
	${CC} ${FILES} -o ArchiMaker ${CFLAGS}

debug: ${FILES}
	${CC} ${FILES} -o debug ${CFLAGS} ${DEBUG}

clean:
	${RM} *.o ArchiMaker debug
