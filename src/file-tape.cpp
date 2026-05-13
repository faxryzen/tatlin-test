#include "file-tape.hpp"

#include <filesystem>
#include <thread>
#include <chrono>

using ms = std::chrono::milliseconds;
using sc = std::chrono::steady_clock;

void FileTape::setHeadPos(std::streampos position)
{
  file_.seekg(position);
  file_.seekp(position);
  pos_ = position;
}

FileTape::FileTape(const std::string & filepath, const std::string & config):
  pos_(0)
{
  file_.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
  if (!file_.is_open())
  {
    std::ofstream create(filepath, std::ios::binary);
    create.close();
    file_.open(filepath, std::ios::in | std::ios::out | std::ios::binary);
  }
  execConfig(config);
  file_.seekg(0, std::ios::end);
  file_size_ = file_.tellg();
  setHeadPos(pos_);
}

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

bool FileTape::execConfig(const std::string & config) noexcept
{
  using json = nlohmann::json;
  try
  {
    std::ifstream raw_cfg(config);
    if (!raw_cfg)
    {
      throw std::runtime_error("filetape: cfg error: no such file");
    }
    auto cfg = json::parse(raw_cfg);
    if (!cfg.contains("delays"))
    {
      throw std::runtime_error("filetape: cfg error: invalid json");
    }
    auto delays = cfg["delays"];
    delays_["read"] = delays.value("read", 20);
    delays_["write"] = delays.value("read", 20);
    delays_["rewind"] = delays.value("read", 100);
    delays_["move"] = delays.value("read", 10);
  }
  catch (const std::exception & e)
  {
    delays_["read"] = 5;
    delays_["write"] = 5;
    delays_["rewind"] = 30;
    delays_["move"] = 2;
    return false;
  }
  return true;
}

std::unique_ptr< Tape > TempFileTapeCreator::create()
{
  std::string filename = "tmp/filetape_" + std::to_string(sc::now().time_since_epoch().count()) + ".tmp";
  std::ofstream file(filename);
  file.close();
  created_files_.push_back(filename);
  return std::make_unique< FileTape >(filename, config_);
}

TempFileTapeCreator::~TempFileTapeCreator()
{
  for (const auto & f: created_files_)
  {
    if (std::filesystem::exists(f))
    {
      std::filesystem::remove(f);
    }
  }
}

TempFileTapeCreator::TempFileTapeCreator(const std::string & configuration):
  config_(configuration)
{}
