#include "file-tape.hpp"

#include <thread>
#include <chrono>

void FileTape::read(int & value)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_["read"]));
  file_.read(reinterpret_cast< char * >(&value), sizeof(value));
  if (!file_)
  {
    return; //todo
  }
  setHeadPos(pos_);
}

void FileTape::write(int value)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_["write"]));
  file_.write(reinterpret_cast< char * >(&value), sizeof(value));
  if (!file_)
  {
    return; //todo
  }
  setHeadPos(pos_);
}

void FileTape::next()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_["move"]));
  setHeadPos(pos_ + static_cast< std::streamoff >(sizeof(int))); //todo eof
}

void FileTape::prev()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_["move"]));
  setHeadPos(pos_ - static_cast< std::streamoff >(sizeof(int)));  //todo negative
}

void FileTape::rewind()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(delays_["rewind"]));
  setHeadPos(0);
}

bool FileTape::end()
{
  if (!file_)
  {
    return true;
  }
  return false;
}
