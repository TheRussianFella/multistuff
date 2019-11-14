#ifndef DRIVER_HH
# define DRIVER_HH
# include <string>
# include <map>
# include <vector>

# include "parser.hh"
# include "pipepart.hpp"

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
public:
  Driver ();

  std::vector<PipePart> result;

  // Run parser on certain string
  int parse (const std::string& target);
  std::string target;

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
