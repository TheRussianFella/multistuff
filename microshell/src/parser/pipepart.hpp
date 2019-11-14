#include <string>
#include <vector>
#include <unistd.h>
#include <stdio.h>

#ifndef PIPEPART
#define PIPEPART

struct PipePart {

public:

  PipePart() : input(0), output(1), input_file(""),
               output_file(""), command(""){};

  int input;
  int output;

  std::string input_file;
  std::string output_file;

  std::string command;
  std::vector<std::string> arguments;

  int init() {
    command = arguments[0];
    return 0;
  }

};

#endif
