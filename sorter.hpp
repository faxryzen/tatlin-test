#ifndef SORTER_HPP
#define SORTER_HPP

#include <fstream>
#include <cstddef>
#include <array>

#include <tape.hpp>

class Sorter
{
private:
  static constexpr size_t MEMORY_SIZE = 10;

  Tape * in_;
  Tape * out_;

  std::array< int, MEMORY_SIZE > mem_;

  size_t partitionMemory(size_t left, size_t right)
  {
    int pivot = mem_[right];
    size_t i = (left - 1);

    for (size_t j = left; j <= right - 1; j++)
    {
      if (mem_[j] <= pivot)
      {
        i++;
        std::swap(mem_[i], mem_[j]);
      }
    }

    std::swap(mem_[i + 1], mem_[right]);
    return (i + 1);
  }

  void sortInMemory(size_t left, size_t right)
  {
    if (left < right)
    {
      size_t pi = partitionMemory(left, right);
      sortInMemory(left, pi - 1);
      sortInMemory(pi + 1, right);
    }
  }
public:
  Sorter(Tape * input, Tape * output):
    in_(input),
    out_(output)
  {}

  void sort()
  {
    size_t i = 0;
    for (; i < MEMORY_SIZE; ++i)
    {
      int vi = 0;
      if (in_->end())
      {
        break;
      }
      in_->read(vi);
      mem_[i] = vi;
      in_->next();
    }
    sortInMemory(0, i);
    i = 0;
    std::ofstream temp("tmp/temp");
    //here
    for (; i < MEMORY_SIZE; ++i)
    {
      out_->write(mem_[i]);
      out_->next();
    }
  }
};

#endif
