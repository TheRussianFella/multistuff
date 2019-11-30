#include <iostream>

#include "microsh.h"

int main(int argc, char** argv) {

  Microsh micro;
  std::string line;
  int code = 0;

  if (argc == 1){
    while (!code) {
      line = micro.get_line();
      code = micro.run(line);
    }
  } else if (std::string(argv[1]) == "test") {

    std::vector<std::string> tests({ std::string("ls -l"),
    std::string("ls -l *"),
    std::string("ls -l /*"),
    std::string("ls -l /*/*"),
    std::string("ls -l /*/*.h"),
    std::string("ls -l /usr/include/*.h"),
    std::string("cat < Makefile | cat | cat | cat | cat"),
    std::string("cat < Makefile | cat | cat | cat | cat | cat > b"),
    std::string("cat < a < b"),
    std::string("cat < Makefile | cat > b"),
    std::string("set MAX=idon'tlikeyou"),
    std::string("echo $MAX")
  });

    for (auto t: tests)
      micro.run(t);

  }

  return 0;
}
