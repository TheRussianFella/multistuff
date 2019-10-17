#include "microsh.h"

// Constructor
Microsh::Microsh() : curr_dir("/"), reg_parser(), lex_parser() {};

// Methods

std::string Microsh::get_line() {

  std::string line;
  std::cout << this->curr_dir << " > ";
  std::getline(std::cin, line);

  return line;
};

std::string Microsh::parse_reg_exp(const std::string &line) {
  return this->reg_parser.parse(line);
}

std::vector<PipePart> Microsh::parse_command(const std::string &line) {
  return this->lex_parser.parse(line);
}

///////////////
// Execution //
///////////////

int Microsh::execute_pipe(std::vector<PipePart>& parts, int last_output, size_t idx) {

  // Check if pipe has ended and then connect it's parts

  if (idx != parts.size()) {

    int output_pipe[2];
    pipe(output_pipe);

    // Child process
    if ( fork() == 0 ) {

      if (last_output || parts[idx].input != 0) {
        dup2( (parts[idx].input == 0) ? last_output : parts[idx].input, 0);
        if (last_output)
          close(last_output);
      }

      if ((idx != parts.size()-1) || (parts[idx].output != 1))
        dup2((parts[idx].output == 1) ? output_pipe[1] : parts[idx].output, 1);

      close(output_pipe[0]);
      close(output_pipe[1]);

      parts[idx].exec();

    // Parrent
    } else {

      if (last_output)
        close(last_output);
      close(output_pipe[1]);

      return execute_pipe(parts, output_pipe[0], idx+1);
    }
  }

  return 0;
}

int Microsh::run(const std::string& line) {

  // Create parts of pipe

  std::vector<PipePart> parts = this->lex_parser.parse(line);

  // Now pipe itself

  int code = execute_pipe(parts, 0, 0);

  // Now wait for it all to end

  int status;
  for (size_t i = 0; i < parts.size(); ++i)
    wait(&status);

  return code;
};
