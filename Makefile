CC = gcc
SRC = *.c
FLAGS = -Wall -O2 -o 
BIN = inspect

${BIN} : ${SRC}
	${CC} ${SRC} ${FLAGS} ${BIN}
	cp ${BIN} /usr/bin/i 
	mv ${BIN} /usr/bin/ 
	
