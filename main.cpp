#include <iostream>

#include "file-tape.hpp"

int main()
{
  FileTape tape("test_tape.txt", "config.json");
  std::cout << "read that bitch\n";
  tape.write(10);
  std::cout << "writed 10\n";
  tape.next();
  std::cout << "moved\n";
  tape.write(20);
  std::cout << "writed 20\n";
  tape.next();
  std::cout << "moved\n";
  tape.write(30);
  std::cout << "writed 30\n";
}
