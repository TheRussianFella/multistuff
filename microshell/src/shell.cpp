#include <iostream>

#include "microsh.h"

int main() {

  Microsh micro;
  std::string line;
  int code = 0;

  while (!code) {

    line = micro.get_line();
    code = micro.run(line);

  }

  return 0;
}
