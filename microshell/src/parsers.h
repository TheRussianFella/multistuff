#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include <unistd.h>
#include <stdio.h>


// Regular expressions parser

class RegExpParser {

public:
  RegExpParser();
  std::string parse(const std::string& line);

};

// Lexem parser

struct PipePart {

public:

  PipePart();
  PipePart(const std::string& line);

  int input;
  int output;
  std::string command;
  std::vector<std::string> arguments;

  int exec();

};

class LexParser {

public:
  LexParser();
  std::vector<PipePart> parse(const std::string& line);
};
