#include <iostream>
#include "driver.hh"

int
main (int argc, char *argv[])
{
  int res = 0;
  Driver drv;

  drv.parse("ls -l > a.txt | grep \"sdfjklj sdfjlk\" | cat < a < b");

  for (auto a: drv.result) {
    std::cout << "\nCommand: " << a.command << "\n";
    std::cout << "Args: ";
    for (auto b: a.arguments)
      std::cout << b << " ";
    std::cout << "\nInp: " << a.input_file << "\n";
    std::cout << "Out: " << a.output_file << "\n";

  }

  return res;
}
