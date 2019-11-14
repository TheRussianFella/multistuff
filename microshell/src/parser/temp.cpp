#include <iostream>
#include "driver.hh"
#include "defaultdict.hpp"

int
main (int argc, char *argv[])
{
  int res = 0;
  DefaultDict* vars = new DefaultDict();

  Driver drv(vars);

  drv.parse("ls -l > a.txt | grep \"sdfjklj sdfjlk\" | cat < a < b");

  for (auto a: drv.result) {
    std::cout << "\nCommand: " << a.command << "\n";
    std::cout << "Args: ";
    for (auto b: a.arguments)
      std::cout << b << " ";
    std::cout << "\nInp: " << a.input_file << "\n";
    std::cout << "Out: " << a.output_file << "\n";

  }

  delete vars;
  
  return res;
}
