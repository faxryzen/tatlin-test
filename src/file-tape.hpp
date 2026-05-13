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

  void setHeadPos(std::streampos position);
public:
  FileTape(const std::string & filepath, const std::string & config);

  void read(int & value) override;
  void write(int value) override;

  void next() override;
  void prev() override;
  void rewind() override;

  bool end() override;
private:
  bool execConfig(const std::string & config) noexcept;
};

class TempFileTapeCreator: public TempTapeCreator
{
  std::vector< std::string > created_files_;
  std::string config_ = "configs/default_delays.json";
public:
  ~TempFileTapeCreator();
  TempFileTapeCreator() = default;
  TempFileTapeCreator(const std::string & configuration);
  std::unique_ptr< Tape > create() override;
};

#endif
