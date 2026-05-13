#include <iostream>

#include "file-tape.hpp"
#include "sorter.hpp"

int main(int argc, char ** argv)
{
  if (argc != 3)
  {
    std::cerr << "format: ./output <tape1> <tape2>\n";
    return 1;
  }
  std::ofstream tapefile1(argv[1]);
  tapefile1.close();
  std::ofstream tapefile2(argv[2]);
  tapefile2.close();
  FileTape tape1(argv[1], "configs/config.json");
  FileTape tape2(argv[2], "configs/config.json");
  savintsev::TapeSorter sorter;
  TempFileTapeCreator creator;
  sorter.sort(&tape1, &tape2, &creator);
}
