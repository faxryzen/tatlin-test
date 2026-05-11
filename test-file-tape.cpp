#include <boost/test/unit_test.hpp>
#include "file-tape.hpp"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

struct TempCreater
{
private:
  bool existed_ = false;
public:
  TempCreater():
    existed_(fs::exists("tmp"))
  {
    if (!existed_)
    {
      fs::create_directory("tmp");
    }
  }
  ~TempCreater()
  {
    if (!existed_)
    {
      fs::remove("tmp");
    }
  }
};

BOOST_GLOBAL_FIXTURE(TempCreater);

BOOST_AUTO_TEST_CASE(simple_write_and_read)
{
  std::ofstream file("tmp/simple_write_and_read");
  BOOST_TEST(file.is_open());
  file.close();

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
