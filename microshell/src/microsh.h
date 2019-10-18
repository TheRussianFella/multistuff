#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "parsers.h"

class Microsh {

private:

  std::string curr_dir;
  RegExpParser reg_parser;
  LexParser lex_parser;

  std::vector<PipePart> parse_command(const std::string& line);
  int exec_pipe(const std::vector<PipePart>& parts, int last_output, size_t idx);
  int exec(const PipePart& part);

public:

  Microsh();

  std::string get_line();
  std::string parse_reg_exp(const std::string& line);
  int run(const std::string& line);

};
