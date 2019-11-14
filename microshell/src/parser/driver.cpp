#include "driver.hh"
#include "parser.hh"

//////
// Parsing driver
//////

Driver::Driver ()
  : trace_parsing (false), trace_scanning (false) {}

int Driver::parse (const std::string& target) {

  this->target = target + "\n";
  location.initialize (&(this->target));
  scan_begin ();
  yy::parser parse (*this);
  parse.set_debug_level (trace_parsing);
  int res = parse ();
  scan_end ();
  return res;

}
