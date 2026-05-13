#ifndef SORTER_HPP
#define SORTER_HPP

#include <algorithm>
#include <cstddef>
#include <vector>
#include <array>

#include "tape.hpp"

namespace savintsev
{
  class TapeSorter
  {
  private:
    //how much int s machine can handle
    const size_t MEMORY_SIZE;
    //how much temp tapes can be created
    //works only with 3
    static constexpr size_t TEMP_TAPES = 3;

    Tape * in_;
    Tape * out_;

    std::vector< int > mem_;
    std::array< std::unique_ptr< Tape >, TEMP_TAPES > tmp_;

  public:
    TapeSorter(size_t memory_size = 100):
      MEMORY_SIZE(memory_size),
      mem_(MEMORY_SIZE)
    {}

    template< typename Compare = std::less< int > >
    void sort(Tape * src, Tape * dst, TempTapeCreator * creator, Compare comp = Compare())
    {
      if (!src || !dst)
      {
        return;
      }
      in_ = src;
      out_ = dst;

      in_->rewind();

      size_t input = 0;
      while (input < MEMORY_SIZE && !in_->end())
      {
        in_->read(mem_[input]);
        in_->next();
        ++input;
      }
      if (input < MEMORY_SIZE)
      {
        std::sort(mem_.begin(), mem_.begin() + input, Compare{});
        for (size_t i = 0; i < input; ++i)
        {
          out_->write(mem_[i]);
          out_->next();
        }
        return;
      }

      if (!creator)
      {
        return;
      }

      tmp_[0] = creator->create();
      tmp_[1] = creator->create();
      tmp_[2] = creator->create();

      std::array< size_t, TEMP_TAPES > runs;
      runs.fill(0);

      size_t k = 0;

      while (input > 0)
      {
        std::sort(mem_.begin(), mem_.begin() + input, Compare{});

        tmp_[k]->write(input);
        tmp_[k]->next();

        for (size_t i = 0; i < input; ++i)
        {
          tmp_[k]->write(mem_[i]);
          tmp_[k]->next();
        }
        runs[k]++;
        k = (k == 0) ? 1 : 0;
        input = 0;
        while (input < MEMORY_SIZE && !in_->end())
        {
          in_->read(mem_[input]);
          in_->next();
          ++input;
        }
      }

      std::array< size_t, TEMP_TAPES > fake_runs;
      fake_runs.fill(0);

      size_t total = runs[0] + runs[1];
      size_t smaller = 0, larger = 1;
      while (smaller + larger < total)
      {
        size_t next = smaller + larger;
        smaller = larger;
        larger = next;
      }

      while (smaller < std::min(runs[0], runs[1]))
      {
        size_t next = smaller + larger;
        smaller = larger;
        larger = next;
      }

      if (runs[0] >= runs[1])
      {
        fake_runs[0] = larger - runs[0];
        fake_runs[1] = smaller - runs[1];
      }
      else
      {
        fake_runs[0] = smaller - runs[0];
        fake_runs[1] = larger - runs[1];
      }

      size_t src1 = 0;
      size_t src2 = 1;
      size_t dest = 2;

      tmp_[src1]->rewind();
      tmp_[src2]->rewind();

      while (runs[0] + runs[1] + runs[2] > 1)
      {
        tmp_[dest]->rewind();
        while ((fake_runs[src1] || runs[src1]) && (fake_runs[src2] || runs[src2]))
        {
          if (fake_runs[src1] > 0 && fake_runs[src2] > 0)
          {
            --fake_runs[src1];
            --fake_runs[src2];
            ++fake_runs[dest];
          }
          else if (fake_runs[src1] > 0 && runs[src2] > 0)
          {
            size_t i = 0;
            int n = 0;
            tmp_[src2]->read(n);
            tmp_[src2]->next();
            tmp_[dest]->write(n);
            tmp_[dest]->next();
            while (i < n)
            {
              int val = 0;
              tmp_[src2]->read(val);
              tmp_[src2]->next();
              tmp_[dest]->write(val);
              tmp_[dest]->next();
              ++i;
            }
            --fake_runs[src1];
            --runs[src2];
            ++runs[dest];
          }
          else if (runs[src1] > 0 && fake_runs[src2] > 0)
          {
            size_t i = 0;
            int n = 0;
            tmp_[src2]->read(n);
            tmp_[src2]->next();
            tmp_[dest]->write(n);
            tmp_[dest]->next();
            while (i < n)
            {
              int val = 0;
              tmp_[src1]->read(val);
              tmp_[src1]->next();
              tmp_[dest]->write(val);
              tmp_[dest]->next();
              ++i;
            }
            --fake_runs[src2];
            --runs[src1];
            ++runs[dest];
          }
          else if (runs[src1] > 0 && runs[src2] > 0)
          {
            size_t i1 = 0, i2 = 0;
            int n1 = 0, n2 = 0;
            int val1 = 0, val2 = 0;

            tmp_[src1]->read(n1);
            tmp_[src1]->next();
            tmp_[src2]->read(n2);
            tmp_[src2]->next();
            tmp_[dest]->write(n1 + n2);
            tmp_[dest]->next();

            while (i1 < n1 && i2 < n2)
            {
              tmp_[src1]->read(val1);
              tmp_[src2]->read(val2);
              if (Compare{}(val1, val2))
              {
                tmp_[dest]->write(val1);
                tmp_[src1]->next();
                ++i1;
              }
              else
              {
                tmp_[dest]->write(val2);
                tmp_[src2]->next();
                ++i2;
              }
              tmp_[dest]->next();
            }
            while (i1 < n1)
            {
              int val = 0;
              tmp_[src1]->read(val);
              tmp_[src1]->next();
              tmp_[dest]->write(val);
              tmp_[dest]->next();
              ++i1;
            }
            while (i2 < n2)
            {
              int val = 0;
              tmp_[src2]->read(val);
              tmp_[src2]->next();
              tmp_[dest]->write(val);
              tmp_[dest]->next();
              ++i2;
            }
            --runs[src1];
            --runs[src2];
            ++runs[dest];
          }
        }
        if (!runs[src1] && !fake_runs[src1])
        {
          std::swap(src1, dest);
          tmp_[src1]->rewind();
        }
        else
        {
          std::swap(src2, dest);
          tmp_[src2]->rewind();
        }
      }
      size_t final = runs[src1] > 0 ? src1 : src2;
      tmp_[final]->rewind();
      while (!tmp_[final]->end())
      {
        int n = 0;
        tmp_[final]->read(n);
        tmp_[final]->next();
        for (size_t i = 0; i < n; ++i)
        {
          int val = 0;
          tmp_[final]->read(val);
          tmp_[final]->next();
          out_->write(val);
          out_->next();
        }
      }
      out_->rewind();
    }
  };
}

#endif
