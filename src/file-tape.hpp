#ifndef FILE_TAPE_HPP
#define FILE_TAPE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "tape.hpp"

class FileTape: public Tape
{
private:
  std::fstream file_;
  std::streampos pos_;
  std::streampos file_size_;
  std::unordered_map< std::string, size_t > delays_;

  void setHeadPos(std::streampos position)
  {
    file_.seekg(position);
    file_.seekp(position);
    pos_ = position;
  }
public:
  FileTape(const std::string & filepath, const std::string & config):
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

  void read(int & value) override;
  void write(int value) override;

  void next() override;
  void prev() override;
  void rewind() override;

  bool end() override;
private:
  bool execConfig(const std::string & config)
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
    catch (const std::exception & e) //json::exception impl std::exception
    {
      delays_["read"] = 20;
      delays_["write"] = 20;
      delays_["rewind"] = 100;
      delays_["move"] = 10;
      return false;
    }
    return true;
  }
};

class TempFileTapeCreator: public TempTapeCreator
{
  std::string config_ = "configs/default_delays.json";
public:
  TempFileTapeCreator() = default;
  TempFileTapeCreator(const std::string & configuration);
  std::unique_ptr< Tape > create() override;
};

#endif
