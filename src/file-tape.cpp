#include "file-tape.hpp"

#include <filesystem>
#include <thread>
#include <chrono>

using ms = std::chrono::milliseconds;
using sc = std::chrono::steady_clock;

void FileTape::read(int & value)
{
  std::this_thread::sleep_for(ms(delays_["read"]));
  file_.read(reinterpret_cast< char * >(&value), sizeof(value));
  if (!file_)
  {
    return; //todo
  }
  setHeadPos(pos_);
}

void FileTape::write(int value)
{
  std::this_thread::sleep_for(ms(delays_["write"]));
  file_.write(reinterpret_cast< char * >(&value), sizeof(value));
  std::streampos current_end = file_.tellp();
  if (current_end > file_size_)
  {
    file_size_ = current_end;
  }
  setHeadPos(pos_);
}

void FileTape::next()
{
  std::this_thread::sleep_for(ms(delays_["move"]));
  setHeadPos(pos_ + static_cast< std::streamoff >(sizeof(int))); //todo eof
}

void FileTape::prev()
{
  std::this_thread::sleep_for(ms(delays_["move"]));
  setHeadPos(pos_ - static_cast< std::streamoff >(sizeof(int)));  //todo negative
}

void FileTape::rewind()
{
  std::this_thread::sleep_for(ms(delays_["rewind"]));
  file_.clear();
  setHeadPos(0);
}

bool FileTape::end()
{
  return pos_ >= file_size_;
}

std::unique_ptr< Tape > TempFileTapeCreator::create()
{
  std::string filename = "tmp/filetape_" + std::to_string(sc::now().time_since_epoch().count()) + ".tmp";
  std::ofstream file(filename);
  file.close();
  return std::make_unique< FileTape >(filename, config_);
}

TempFileTapeCreator::TempFileTapeCreator(const std::string & configuration):
  config_(configuration)
{}
