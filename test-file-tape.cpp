#include <boost/test/unit_test.hpp>
#include "file-tape.hpp"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

struct TempCreator
{
private:
  bool existed_ = false;
public:
  TempCreator():
    existed_(fs::exists("tmp"))
  {
    if (!existed_)
    {
      fs::create_directory("tmp");
    }
  }
  ~TempCreator()
  {
    if (!existed_)
    {
      fs::remove("tmp");
    }
  }
};

struct TestFileCreator
{
private:
  fs::path filepath_;
  bool existed_ = false;
  bool error_ = false;
public:
  TestFileCreator(const std::string & filepath):
    filepath_(filepath)
  {
    if (fs::exists(filepath_))
    {
      fs::remove(filepath_);
    }
    std::ofstream file(filepath_);
    if (!file)
    {
      error_ = true;
      return;
    }
    file.close();
  }
  ~TestFileCreator()
  {
    if (!error_ && fs::exists(filepath_))
    {
      fs::remove(filepath_);
    }
  }
  bool isGood()
  {
    return !error_;
  }
};

BOOST_GLOBAL_FIXTURE(TempCreator);

BOOST_AUTO_TEST_CASE(simple_write_and_read)
{
  TestFileCreator test("tmp/simple_write_and_read");
  BOOST_TEST(test.isGood());

  FileTape tape("tmp/simple_write_and_read", "config.json");

  tape.write(10);
  int value = 0;
  tape.read(value);
  BOOST_TEST(value == 10);

  tape.write(20);
  tape.read(value);
  BOOST_TEST(value == 20);

  tape.next();
  tape.write(30);
  tape.next();
  tape.write(40);
  tape.next();
  tape.write(5555);

  tape.read(value);
  BOOST_TEST(value == 5555);
  tape.prev();
  tape.read(value);
  BOOST_TEST(value == 40);
  
  tape.rewind();
  tape.read(value);
  BOOST_TEST(value == 20);
  tape.next();
  tape.read(value);
  BOOST_TEST(value == 30);
}
