.DEFAULT_GOAL := build

#========
# Constants
#========
BIN = bin
CXXFLAGS = -g -Wall -fsanitize=address -std=c++17 -pthread
CXX = g++

#========
# Build
#========

${BIN}/parallel_queue.o: parallel_queue.cpp parallel_queue.h
	mkdir -p ${BIN}
	${CXX} ${CXXFLAGS} -c parallel_queue.cpp -o ${BIN}/parallel_queue.o

build: parsearch.cpp crawler.hpp matcher.hpp
	${CXX} ${CXXFLAGS} parsearch.cpp -o search -lstdc++fs
