#include "parsers.h"

// RegExpParser

RegExpParser::RegExpParser() {};
std::string RegExpParser::parse(const std::string &line) {
  return std::string( line );
}

// LexParser

PipePart::PipePart() {};

PipePart::PipePart(const std::string& line) {

  // Parse arguments

  size_t output_pos = line.find_first_of(">");
  size_t input_pos =  line.find_first_of("<");

  std::stringstream ss(line.substr(0, std::min(output_pos, input_pos) ));
  std::string temp;

  while (ss >> temp)
    arguments.push_back(temp);

  command = arguments[0];

  // Parse input and output files (if given)
  // Comment: When working with file just open it here assign it's file descriptor

  input = 0;
  output = 1;

  output_pos = line.find_last_of(">");
  input_pos  = line.find_last_of("<");

  if (output_pos != std::string::npos) {
    output_pos = line.find_first_not_of(" ", output_pos+1);
    std::string filename = line.substr(output_pos, line.find(" ", output_pos) - output_pos);
    output = fileno(fopen(filename.c_str(), "w+"));
  }

  if (input_pos != std::string::npos) {
    input_pos = line.find_first_not_of(" ", input_pos+1);
    std::string filename = line.substr(input_pos, line.find(" ", input_pos) - input_pos);
    input = fileno(fopen(filename.c_str(), "r+"));
  }

}

LexParser::LexParser() {};

//TODO: If file doesn't exist we should not crash

std::vector<PipePart> LexParser::parse(const std::string& line) {

  std::vector<PipePart> commands;

  std::string delim = "|";

  auto start = 0U;
  auto end = line.find(delim);

  while (end != std::string::npos)
  {
      commands.push_back( PipePart( line.substr(start, end - start) ) );

      start = end + delim.length();
      end = line.find(delim, start);
  }

  commands.push_back( PipePart( line.substr(start, end) ) );

  return commands;
};
