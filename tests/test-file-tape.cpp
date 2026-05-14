#include <boost/test/unit_test.hpp>
#include "file-tape.hpp"

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

BOOST_GLOBAL_FIXTURE(TempCreator);

BOOST_AUTO_TEST_CASE(simple_write_and_read)
{
  TempFileTapeCreator creator("configs/test_delays.json");
  std::unique_ptr< Tape > tape = creator.create();

  tape->write(10);
  int value = 0;
  tape->read(value);
  BOOST_TEST(value == 10);

  tape->write(20);
  tape->read(value);
  BOOST_TEST(value == 20);

  tape->next();
  tape->write(30);
  tape->next();
  tape->write(40);
  tape->next();
  tape->write(5555);

  tape->read(value);
  BOOST_TEST(value == 5555);
  tape->prev();
  tape->read(value);
  BOOST_TEST(value == 40);
  
  tape->rewind();
  tape->read(value);
  BOOST_TEST(value == 20);
  tape->next();
  tape->read(value);
  BOOST_TEST(value == 30);
}

BOOST_AUTO_TEST_CASE(eof_and_rewind)
{
  TempFileTapeCreator creator("configs/test_delays.json");
  std::unique_ptr< Tape > tape = creator.create();

  for (int i = 0; i < 20; ++i)
  {
    tape->write(i * 10);
    tape->next();
  }

  tape->rewind();

  for (int i = 0; i < 20; ++i)
  {
    int val = 0;
    tape->read(val);
    tape->next();
    BOOST_TEST(val == i * 10);
  }

  tape->rewind();
  BOOST_TEST(!tape->end());

  int i = 0;
  int value = 0;
  tape->read(value);
  BOOST_TEST(!tape->end());
  while (!tape->end())
  {
    BOOST_TEST(value == i * 10);
    tape->next();
    tape->read(value);
    ++i;
  }

  tape->rewind();
  BOOST_TEST(!tape->end());

  i = 0;
  tape->read(value);
  BOOST_TEST(!tape->end());
  while (!tape->end())
  {
    BOOST_TEST(value == i * 10);
    tape->next();
    tape->read(value);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(no_config)
{
  TempFileTapeCreator creator("configs/test_delays.json");
  std::unique_ptr< Tape > tape = creator.create();

  for (int i = 0; i < 20; ++i)
  {
    tape->write(i * 10);
    tape->next();
  }

  tape->rewind();

  for (int i = 0; i < 20; ++i)
  {
    int val = 0;
    tape->read(val);
    tape->next();
    BOOST_TEST(val == i * 10);
  }

  tape->rewind();
  BOOST_TEST(!tape->end());

  int i = 0;
  int value = 0;
  tape->read(value);
  BOOST_TEST(!tape->end());
  while (!tape->end())
  {
    BOOST_TEST(value == i * 10);
    tape->next();
    tape->read(value);
    ++i;
  }

  tape->rewind();
  BOOST_TEST(!tape->end());

  i = 0;
  tape->read(value);
  BOOST_TEST(!tape->end());
  while (!tape->end())
  {
    BOOST_TEST(value == i * 10);
    tape->next();
    tape->read(value);
    ++i;
  }
}

BOOST_AUTO_TEST_CASE(test_prev)
{
  TempFileTapeCreator creator("configs/test_delays.json");
  auto tape = creator.create();

  int values[] = {10, 20, 30, 40, 50};
  for (int v : values)
  {
    tape->write(v);
    tape->next();
  }

  tape->rewind();

  int val;
  tape->read(val);
  BOOST_TEST(val == 10);

  tape->next();
  tape->read(val);
  BOOST_TEST(val == 20);

  tape->next();
  tape->read(val);
  BOOST_TEST(val == 30);

  tape->prev();
  tape->read(val);
  BOOST_TEST(val == 20);

  tape->prev();
  tape->read(val);
  BOOST_TEST(val == 10);

  tape->prev();
  tape->read(val);
  BOOST_TEST(val == 10);

  tape->prev();
  tape->read(val);
  BOOST_TEST(val == 10);

  tape->next();
  tape->next();
  tape->next();
  tape->next();
  tape->read(val);
  BOOST_TEST(val == 50);

  tape->next();
  BOOST_TEST(tape->end() == true);
}
