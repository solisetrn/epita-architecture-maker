CC = gcc
CFLAGS = -Wall -Wextra -g
DEBUG = -fsanitize=address
FILES = main.o tree_manager.o
RM = rm -f

all: ${FILES}
	${CC} ${FILES} -o ArchiMaker ${CFLAGS}

debug: ${FILES}
	${CC} ${FILES} -o debug ${CFLAGS} ${DEBUG}

save:
	git add -A
	git commit -m "save"
	git push

clean:
	${RM} *.o ArchiMaker debug
