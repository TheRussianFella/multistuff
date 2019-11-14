.DEFAULT_GOAL := build

#========
# Constants
#========
SRC = src
BIN = bin
PARSER_PATH = src/parser
CCFLAGS = -g -Wall -fsanitize=address
CXX = g++

#========
# Parser
#========

BISON = bison
FLEX = flex

${BIN}/parser.o: ${PARSER_PATH}/parser.yy
	${BISON} -o ${PARSER_PATH}/parser.cc ${PARSER_PATH}/parser.yy
	${CXX} ${CCFLAGS} -c ${PARSER_PATH}/parser.cc -o ${BIN}/parser.o

${BIN}/driver.o: ${PARSER_PATH}/driver.cpp ${PARSER_PATH}/driver.hh ${BIN}/parser.o
	${CXX} ${CCFLAGS} -c ${PARSER_PATH}/driver.cpp -o ${BIN}/driver.o

${BIN}/scanner.o: ${PARSER_PATH}/scanner.ll ${BIN}/parser.o
	${FLEX} -o${PARSER_PATH}/scanner.cc ${PARSER_PATH}/scanner.ll
	${CXX} ${CCFLAGS} -c ${PARSER_PATH}/scanner.cc -o ${BIN}/scanner.o

${BIN}/temp.o: ${PARSER_PATH}/temp.cpp ${BIN}/driver.o
	${CXX} ${CCFLAGS} -c ${PARSER_PATH}/temp.cpp -o ${BIN}/temp.o

test_parser: ${BIN}/temp.o ${BIN}/scanner.o ${BIN}/parser.o ${BIN}/driver.o
	${CXX} ${CCFLAGS} -o pars ${BIN}/temp.o \
	                                 ${BIN}/scanner.o ${BIN}/parser.o ${BIN}/driver.o

#========
# Build
#========

${BIN}/microsh.o: ${SRC}/microsh.cpp ${SRC}/microsh.h
	${CXX} ${CCFLAGS} -c ${SRC}/microsh.cpp -o ${BIN}/microsh.o

${BIN}/parsers.o: ${SRC}/parsers.cpp ${SRC}/parsers.h
	${CXX} ${CCFLAGS} -c ${SRC}/parsers.cpp -o ${BIN}/parsers.o

${BIN}/shell.o: ${BIN}/microsh.o ${BIN}/parsers.o ${SRC}/shell.cpp
	${CXX} ${CCFLAGS} -c ${SRC}/shell.cpp -o ${BIN}/shell.o


build: ${BIN}/microsh.o ${BIN}/shell.o ${BIN}/parsers.o
	${CXX} ${CCFLAGS} -o shell ${BIN}/microsh.o ${BIN}/parsers.o ${BIN}/shell.o
