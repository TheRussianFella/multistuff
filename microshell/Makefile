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

${BIN}/parser.o: ${PARSER_PATH}/parser.yy ${PARSER_PATH}/pipepart.hpp
	${BISON} -o ${PARSER_PATH}/parser.cc ${PARSER_PATH}/parser.yy
	${CXX} ${CCFLAGS} -c ${PARSER_PATH}/parser.cc -o ${BIN}/parser.o

${BIN}/driver.o: ${PARSER_PATH}/driver.cpp ${PARSER_PATH}/driver.hh ${BIN}/parser.o ${PARSER_PATH}/pipepart.hpp
	${CXX} ${CCFLAGS} -c ${PARSER_PATH}/driver.cpp -o ${BIN}/driver.o

${BIN}/scanner.o: ${PARSER_PATH}/scanner.ll ${BIN}/parser.o
	${FLEX} -o${PARSER_PATH}/scanner.cc ${PARSER_PATH}/scanner.ll
	${CXX} ${CCFLAGS} -c ${PARSER_PATH}/scanner.cc -o ${BIN}/scanner.o

build_lib: ${BIN}/scanner.o ${BIN}/driver.o ${BIN}/parser.o
	mkdir -p ${BIN}
	ar rvs ${BIN}/libpars.a ${BIN}/scanner.o ${BIN}/driver.o ${BIN}/parser.o
	rm ${BIN}/scanner.o ${BIN}/driver.o ${BIN}/parser.o ${PARSER_PATH}/scanner.cc \
	                     ${PARSER_PATH}/parser.cc

test_parser: ${PARSER_PATH}/temp.cpp build_lib
	${CXX} ${CCFLAGS} ${PARSER_PATH}/temp.cpp ${BIN}/libpars.a -o pars

#========
# Build
#========

${BIN}/microsh.o: ${SRC}/microsh.cpp ${SRC}/microsh.h
	${CXX} ${CCFLAGS} -c ${SRC}/microsh.cpp -o ${BIN}/microsh.o

build:  ${SRC}/shell.cpp ${BIN}/microsh.o ${BIN}/libpars.a
	mkdir -p ${BIN}
	${CXX} ${CCFLAGS} ${SRC}/shell.cpp -o shell ${BIN}/microsh.o -L./bin -lpars
