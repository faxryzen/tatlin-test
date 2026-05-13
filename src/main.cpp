#include <iostream>
#include <chrono>
#include <limits>
#include <random>
#include <string>

#include "file-tape.hpp"
#include "sorter.hpp"

int main(int argc, char ** argv)
{
  if (argc < 2)
  {
    std::cerr << "Use:\n" << argv[0] << " --help\nto see available commands\n";
    return 1;
  }

  if (std::strcmp(argv[1], "--help") == 0)
  {
    if (argc != 2)
    {
      std::cerr << "Usage: " << argv[0] << " --help\n";
      return 1;
    }
    std::cout << argv[0] << " --generate <filename> <count> - generate file for sort with random numbers\n";
    std::cout << argv[0] << " <input_tape> <output_tape> - sort <input> and write at <output>\n";
    return 0;
  }

  if (std::strcmp(argv[1], "--generate") == 0)
  {
    if (argc != 4)
    {
      std::cerr << "Usage: " << argv[0] << " --generate <filename> <count>\n";
      return 1;
    }

    std::string filename(argv[2]);
    size_t count = std::stoull(argv[3]);

    std::ofstream(filename).close();
    FileTape tape(filename, "configs/default_delays.json");

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution< int > dist(std::numeric_limits< int >::min(),std::numeric_limits< int >::max());

    std::cout << "Generating tape with " << count << " random numbers...\n";
    for (size_t i = 0; i < count; ++i)
    {
      tape.write(dist(rng));
      tape.next();
    }
    std::cout << "Done. Generated file: " << filename << "\n";
    return 0;
  }

  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <input_tape> <output_tape>\n";
    return 1;
  }

  FileTape input(argv[1], "configs/default_delays.json");
  FileTape output(argv[2], "configs/default_delays.json");

  savintsev::TapeSorter sorter;
  TempFileTapeCreator creator("configs/default_delays.json");

  std::cout << "Sorting...\n";

  auto start = std::chrono::high_resolution_clock::now();
  sorter.sort(&input, &output, &creator, std::less< int >());
  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  auto sec = duration.count() / 1000;
  auto ms = duration.count() % 1000;

  std::cout << "Sorted successfully\n";
  std::cout << "Time: " << sec << "." << ms << " s\n";

  std::cout << "Show results? [y/n]: ";
  std::string answer = "";
  std::cin >> answer;
  if (answer == "y" || answer == "Y" || answer == "yes")
  {
    std::cout << "Sorted data on tape:";
    while (!output.end())
    {
      std::cout << " ";
      int v = 0;
      output.read(v);
      std::cout << v;
      output.next();
    }
    std::cout << "\n";
  }

  return 0;
}
