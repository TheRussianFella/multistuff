#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "parser/driver.hh"
#include "parser/pipepart.hpp"
#include "parser/defaultdict.hpp"

///////////////////////////////////////
// Shell class
///////////////////////////////////////

class Microsh {

private:

  ///////
  // Parser
  //////

  Driver driver;

  ///////
  // Shell functions
  ///////

  typedef int (Microsh::*shell_func)(const PipePart& part);
  std::map<std::string, Microsh::shell_func>* shell_functions;

  // State functions - shell functions that change the state of the shell, so
  // they have to be run in the same process

  std::vector<std::string> state_functions;

  int cd  (const PipePart& part);
  int pwd (const PipePart& part);
  int set (const PipePart& part);
  int echo(const PipePart& part);

  ///////
  // Variables
  ///////

  DefaultDict shell_variables;

  ///////
  // Execution
  ///////

  size_t launched_children;

  std::vector<PipePart> parse_command(const std::string& line);
  int exec_pipe(std::vector<PipePart>& parts, int last_output, size_t idx);
  int exec(const PipePart& part);

public:

  Microsh();
  ~Microsh();

  std::string get_line();
  int run(const std::string& line);

};
