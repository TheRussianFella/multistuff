#include "microsh.h"

// Constructor
Microsh::Microsh() : reg_parser(), lex_parser() {

  shell_functions = new std::map<std::string, Microsh::shell_func>;

  (*shell_functions).insert(std::make_pair(std::string("cd"), &Microsh::cd));
  (*shell_functions).insert(std::make_pair(std::string("pwd"), &Microsh::pwd));
  (*shell_functions).insert(std::make_pair(std::string("set"), &Microsh::set));

  state_functions = std::vector<std::string>({"cd", "set"});

};

Microsh::~Microsh() {
  delete shell_functions;
}

// Methods

std::string Microsh::get_line() {

  int BUFFER_SIZE = 150;
  char* dir_buffer = new char[BUFFER_SIZE];
  getcwd(dir_buffer, BUFFER_SIZE);

  std::cout << std::string(dir_buffer) << " > ";

  delete[] dir_buffer;

  std::string line;
  std::getline(std::cin, line);

  return line;
};

std::string Microsh::parse_reg_exp(const std::string &line) {
  return this->reg_parser.parse(line);
}

std::vector<PipePart> Microsh::parse_command(const std::string &line) {
  return this->lex_parser.parse(line);
}


/////////////////////
// Shell functions //
/////////////////////

int Microsh::cd(const PipePart& part) {

  if (part.arguments.size() == 1)
    chdir("/");
  else
    chdir(part.arguments[1].c_str());
  return 0;
}

int Microsh::pwd(const PipePart& part) {

  int BUFFER_SIZE = 150;
  char* dir_buffer = new char[BUFFER_SIZE];
  getcwd(dir_buffer, BUFFER_SIZE);

  std::cout << std::string(dir_buffer) << "\n";

  delete[] dir_buffer;

  return 0;
}

int Microsh::set(const PipePart& part) {
  return 0;
}

///////////////
// Execution //
///////////////

//TODO: Variables + (set, echo)
//TODO: Insert variables into commands
//TODO: Make display of sudo priveledges

int Microsh::exec(const PipePart& part) {

  ////
  // Shell functions
  ////

  if ( shell_functions -> count(part.command) ) {

    shell_func func = (*shell_functions)[part.command];
    int code = (this->*func)(part);

    exit(code);
  }

  ////
  // System functions
  ///

  // Formatting arguments

  std::vector<char *> args_pointers;

  for (auto const &q: part.arguments) {
    args_pointers.push_back((char *)(q.c_str()));
  }
  args_pointers.push_back(NULL);

  // Execute

  return execvp(part.command.c_str(), &args_pointers[0]);
}

int Microsh::exec_pipe(const std::vector<PipePart>& parts, int last_output, size_t idx) {

  // Check if pipe has ended and then connect it's parts

  if (idx != parts.size()) {

    ////
    // State functions are run in the parent process
    ////

    if (std::find(state_functions.begin(), state_functions.end(), parts[idx].command)
            != state_functions.end()) {

      shell_func func = (*shell_functions)[parts[idx].command];
      int code = (this->*func)(parts[idx]);

      return exec_pipe(parts, 0, idx+1);
    }

    ////
    // Others are run in different processes
    ////

    int output_pipe[2];
    pipe(output_pipe);

    // Child process
    if ( fork() == 0 ) {

      // Pipe stdin if neccesary
      if (last_output || parts[idx].input != 0) {
        dup2( (parts[idx].input == 0) ? last_output : parts[idx].input, 0);
        if (last_output)
          close(last_output);
      }

      // Same with stdout
      if ((idx != parts.size()-1) || (parts[idx].output != 1))
        dup2((parts[idx].output == 1) ? output_pipe[1] : parts[idx].output, 1);

      close(output_pipe[0]);
      close(output_pipe[1]);

      exec(parts[idx]);

    // Parrent
    } else {

      if (last_output)
        close(last_output);
      close(output_pipe[1]);

      return exec_pipe(parts, output_pipe[0], idx+1);
    }
  }

  return 0;
}

int Microsh::run(const std::string& line) {

  // Create parts of pipe

  std::vector<PipePart> parts = this->lex_parser.parse(line);

  // Now pipe itself

  int code = exec_pipe(parts, 0, 0);

  // Now wait for it all to end

  int status;
  for (size_t i = 0; i < parts.size(); ++i)
    wait(&status);

  return code;
};
