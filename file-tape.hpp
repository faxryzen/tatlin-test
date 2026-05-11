#ifndef FILE_TAPE_HPP
#define FILE_TAPE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <json.hpp>

#include "tape.hpp"

class FileTape: public Tape
{
private:
  std::fstream file_;
  std::streampos pos_;
  std::unordered_map< std::string, size_t > delays_;

  void setHeadPos(std::streampos position)
  {
    file_.seekg(position);
    file_.seekp(position);
    pos_ = position;
  }
public:
  FileTape(const std::string & filepath, const std::string & config):
    file_(filepath),
    pos_(0)
  {
    execConfig(config);
    if (!file_)
    {
      //todo
    }
    setHeadPos(pos_);
  }

  void read(int & value) override;
  void write(int value) override;

  void next() override;
  void prev() override;

  void rewind() override;
private:
  void execConfig(const std::string & config)
  {
    using json = nlohmann::json;
    try
    {
      std::ifstream raw_cfg(config);
      if (!raw_cfg)
      {
        return; //todo
      }
      auto cfg = json::parse(raw_cfg);
      if (!cfg.contains("delays"))
      {
        return; //todo
      }
      auto delays = cfg["delays"];
      delays_["read"] = delays.value("read", 20);
      delays_["write"] = delays.value("read", 20);
      delays_["rewind"] = delays.value("read", 100);
      delays_["move"] = delays.value("read", 10);
    }
    catch (const std::exception & e)
    {
      return; //todo
    }
  }
};

#endif
