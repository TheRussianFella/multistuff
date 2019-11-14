#include "driver.hh"
#include "parser.hh"

//////
// Parsing driver
//////

Driver::Driver (DefaultDict* variables)
  : trace_scanning (false), trace_parsing (false) {
    this->variables = variables;
  }

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

std::string Driver::insert_variable(const std::string &line) {
  return variables->at(line.substr(1));
}

// Yes, I have written one custom parser - because I am too dump to
// realise how to add it into grammar.

std::string Driver::insert_multi_variables(const std::string &line) {

  std::string result, var;

  size_t last_el = 0, first_space = 0;

  for (size_t i = line.find("$", last_el); i != std::string::npos;
       i = line.find("$", last_el)) {

    result += line.substr( last_el, i-last_el );

    first_space = line.find(" ", i);

    if (first_space != std::string::npos)
      var = insert_variable( line.substr(i, first_space-i) );
    else {
      var = insert_variable( line.substr(i) );
      result += var;
      last_el = line.size();
      break;
    }

    result += var;
    last_el = first_space+1;
  }

  result += line.substr(last_el);

  return result;
}
