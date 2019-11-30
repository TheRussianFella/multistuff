#ifndef DRIVER_HH
# define DRIVER_HH
# include <string>
# include <map>
# include <vector>
# include <regex>

# include "parser.hh"
# include "pipepart.hpp"
# include "defaultdict.hpp"

// Give Flex the prototype of yylex we want ...
# define YY_DECL \
  yy::parser::symbol_type yylex (Driver& drv)
// ... and declare it for the parser's sake.
YY_DECL;

//////
// Parsing driver
//////

class Driver
{

private:

  DefaultDict* variables;

  int parse_part(size_t curr_token, std::string path,
    std::vector<std::string>& tokens, std::vector<std::string>& answer);

public:

  Driver (DefaultDict* variables);

  std::vector<PipePart> result;

  // Run parser on certain string
  int parse (const std::string& target);
  std::string target;

  /////////
  // Variables parser
  /////////
  std::string insert_variable(const std::string& line);
  std::string insert_multi_variables(const std::string& line);

  /////////
  // RegExp Parser
  /////////
  std::vector<std::string> parse_reg(const std::string& line);

  // Handling the scanner.
  void scan_begin ();
  void scan_end ();

  // Whether to generate scanner debug traces.
  bool trace_scanning;
  bool trace_parsing;

  // The token's location used by the scanner.
  yy::location location;
};
#endif // ! DRIVER_HH
