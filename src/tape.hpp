#ifndef TAPE_HPP
#define TAPE_HPP

#include <memory>

class Tape
{
public:
  virtual ~Tape() = default;

  virtual void read(int & value) = 0;
  virtual void write(int value) = 0;

  virtual void next() = 0;
  virtual void prev() = 0;
  virtual void rewind() = 0;

  virtual bool end() = 0;
};

class TempTapeCreator
{
private:
  std::string config_;
public:
  virtual ~TempTapeCreator() = default;
  virtual std::unique_ptr< Tape > create() = 0;
};

#endif
