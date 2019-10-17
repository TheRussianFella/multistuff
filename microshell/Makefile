.DEFAULT_GOAL := build

#========
# Constants
#========
SRC = src
BIN = bin
CCFLAGS = -g -Wall -fsanitize=address

#========
# Build
#========

${BIN}/microsh.o: ${SRC}/microsh.cpp ${SRC}/microsh.h
	g++ ${CCFLAGS} -c ${SRC}/microsh.cpp -o ${BIN}/microsh.o

${BIN}/parsers.o: ${SRC}/parsers.cpp ${SRC}/parsers.h
	g++ ${CCFLAGS} -c ${SRC}/parsers.cpp -o ${BIN}/parsers.o

${BIN}/shell.o: ${BIN}/microsh.o ${BIN}/parsers.o ${SRC}/shell.cpp
	g++ ${CCFLAGS} -c ${SRC}/shell.cpp -o ${BIN}/shell.o


build: ${BIN}/microsh.o ${BIN}/shell.o ${BIN}/parsers.o
	g++ ${CCFLAGS} -o shell ${BIN}/microsh.o ${BIN}/parsers.o ${BIN}/shell.o
