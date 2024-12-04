CC = gcc
SRC = *.c
FLAGS = -Wall -O2 -o
BIN = inspect

${BIN} : ${SRC}
	${CC} ${SRC} ${FLAGS} ${BIN}
	cp ${BIN} ~/.bin/i
	mv ${BIN} ~/.bin
	
