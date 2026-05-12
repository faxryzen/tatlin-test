#include <iostream>

#include "file-tape.hpp"

int main(int argc, char ** argv)
{
  if (argc != 3)
  {
    std::cerr << "format: ./output <tape1> <tape2>\n";
    return 1;
  }
  std::fstream tape1(argv[1]);
  if (!tape1)
  {
    std::cerr << "invalid file\n";
    return 1;
  }
  std::fstream tape2(argv[2]);
  if (!tape2)
  {
    std::cerr << "invalid file\n";
    return 1;
  }
}
