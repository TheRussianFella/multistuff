#include "parsers.h"

// RegExpParser

RegExpParser::RegExpParser() {};
std::string RegExpParser::parse(const std::string &line) {
  return std::string( line );
}

// LexParser

PipePart::PipePart() {};

int PipePart::exec() {

  // Formatting arguments

  std::vector<char *> args_pointers;

  for (auto const &q: arguments) {
    args_pointers.push_back((char *)(q.c_str()));
  }
  args_pointers.push_back(NULL);

  // Execute

  return execvp(command.c_str(), &args_pointers[0]);
};

LexParser::LexParser() {};

//TODO: Incorporate < and >
PipePart LexParser::get_part(const std::string& line) {

  PipePart part;

  std::stringstream ss(line);
  std::string temp;

  while (ss >> temp)
    part.arguments.push_back(temp);

  part.command = part.arguments[0];

  // When working with file just open it here assign it's file descriptor
  part.input = 0;
  part.output = 1;

  return part;

}

//TODO: If file doesn't exist we should not crash

std::vector<PipePart> LexParser::parse(const std::string& line) {

  std::vector<PipePart> commands;

  std::string delim = "|";

  auto start = 0U;
  auto end = line.find(delim);

  while (end != std::string::npos)
  {
      commands.push_back( get_part( line.substr(start, end - start) ) );

      start = end + delim.length();
      end = line.find(delim, start);
  }

  commands.push_back( get_part( line.substr(start, end) ) );

  return commands;
};
