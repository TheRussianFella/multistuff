.DEFAULT_GOAL := build

#========
# Constants
#========
BIN = bin
CCFLAGS = -g -Wall -fsanitize=address -std=c++17 -pthread
CXX = g++

#========
# Build
#========

${BIN}/parallel_queue.o: parallel_queue.cpp parallel_queue.h
	mkdir -p ${BIN}
	${CXX} ${CCFLAGS} -c parallel_queue.cpp -o ${BIN}/parallel_queue.o

build:  parsearch.cpp ${BIN}/parallel_queue.o
	${CXX} ${CCFLAGS} parsearch.cpp -o search ${BIN}/parallel_queue.o
