#include "microsh.h"

////////////////// Shell

// Constructor
Microsh::Microsh() : driver(&shell_variables), launched_children(0) {

  shell_functions = new std::map<std::string, Microsh::shell_func>;

  (*shell_functions).insert(std::make_pair(std::string("cd"), &Microsh::cd));
  (*shell_functions).insert(std::make_pair(std::string("pwd"), &Microsh::pwd));
  (*shell_functions).insert(std::make_pair(std::string("set"), &Microsh::set));
  (*shell_functions).insert(std::make_pair(std::string("echo"), &Microsh::echo));

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


std::vector<PipePart> Microsh::parse_command(const std::string &line) {
  driver.parse(line);
  return driver.result;
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

  shell_variables[part.arguments[1]] = part.arguments[3];

  return 0;
}

int Microsh::echo(const PipePart &part) {

  std::cout << shell_variables.at("nigga") << "\n";

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
    exit((this->*func)(part));

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

  launched_children += 1;
  return execvp(part.command.c_str(), &args_pointers[0]);
}

int Microsh::exec_pipe(std::vector<PipePart>& parts, int last_output, size_t idx) {

  // Check if pipe has ended and then connect it's parts

  if (idx != parts.size()) {

    // Open pipepart's input and output files

    if (parts[idx].output_file != "")
      parts[idx].output = fileno(fopen(parts[idx].output_file.c_str(), "w+"));
    if (parts[idx].input_file != "")
      parts[idx].input  = fileno(fopen(parts[idx].input_file.c_str(),  "r+"));

    ////
    // State functions are run in the parent process
    ////

    if (std::find(state_functions.begin(), state_functions.end(), parts[idx].command)
            != state_functions.end()) {

      shell_func func = (*shell_functions)[parts[idx].command];
      (this->*func)(parts[idx]);

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

  std::vector<PipePart> parts = parse_command(line);

  // Now pipe itself

  int code = exec_pipe(parts, 0, 0);

  // Now wait for it all to end

  int status;
  for (size_t i = 0; i < parts.size(); ++i) {
    wait(&status);
    launched_children -= 1;
  }

  return code;
};
