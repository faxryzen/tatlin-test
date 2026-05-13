#include <boost/test/unit_test.hpp>

#include "file-tape.hpp"
#include "sorter.hpp"

BOOST_AUTO_TEST_CASE(sort_without_creator)
{
  using namespace savintsev;

  TempFileTapeCreator creator("configs/test_delays.json");
  std::unique_ptr< Tape > src = creator.create();
  std::unique_ptr< Tape > dst = creator.create();

  std::vector< int > vec{44, 12, 89, 5, 33, 67};
  for (int v: vec)
  {
    src->write(v);
    src->next();
  }
  src->rewind();

  TapeSorter sorter;
  sorter.sort(src.get(), dst.get(), nullptr, std::less< int >());
  std::sort(vec.begin(), vec.end(), std::less< int >());

  dst->rewind();

  for (int v: vec)
  {
    int val = 0;
    dst->read(val);
    BOOST_TEST(val == v);
    dst->next();
  }
}

BOOST_AUTO_TEST_CASE(sort_big_data_small_ram)
{
  using namespace savintsev;

  TempFileTapeCreator creator("configs/test_delays.json");
  std::unique_ptr< Tape > src = creator.create();
  std::unique_ptr< Tape > dst = creator.create();

  std::vector< int > vec{10, 432, 8, 43, 64, 87, 2, 4, 65, 23, 76, 1, 13, 87, 134, 87, 42, 98, 24,  64, 13, 876, 9, 67843, 3, 643, 6, 346};
  for (int v: vec)
  {
    src->write(v);
    src->next();
  }
  src->rewind();

  TapeSorter sorter(5);
  sorter.sort(src.get(), dst.get(), &creator, std::less< int >());
  std::sort(vec.begin(), vec.end(), std::less< int >());

  dst->rewind();

  for (int v: vec)
  {
    int val = 0;
    dst->read(val);
    BOOST_TEST(val == v);
    dst->next();
  }
}

BOOST_AUTO_TEST_CASE(sort_big_data_small_ram2)
{
  using namespace savintsev;

  TempFileTapeCreator creator("configs/test_delays.json");
  std::unique_ptr< Tape > src = creator.create();
  std::unique_ptr< Tape > dst = creator.create();

  std::vector< int > vec{657, 65, 63, 746, 633, 65, 76, 23, 7, 324, 223, 46, 76};
  for (int v: vec)
  {
    src->write(v);
    src->next();
  }
  src->rewind();

  TapeSorter sorter(3);
  sorter.sort(src.get(), dst.get(), &creator, std::less< int >());
  std::sort(vec.begin(), vec.end(), std::less< int >());

  dst->rewind();

  for (int v: vec)
  {
    int val = 0;
    dst->read(val);
    BOOST_TEST(val == v);
    dst->next();
  }
}

BOOST_AUTO_TEST_CASE(sort_diff_comp)
{
  using namespace savintsev;

  TempFileTapeCreator creator("configs/test_delays.json");
  std::unique_ptr< Tape > src = creator.create();
  std::unique_ptr< Tape > dst = creator.create();

  std::vector< int > vec{44, 12, 89, 5, 33, 67, 3, 3, 13, 87, 35, 426246, 76, 87, 2, 1, 435, 764, 24, 2, 678, 4};
  for (int v: vec)
  {
    src->write(v);
    src->next();
  }
  src->rewind();

  TapeSorter sorter(10);
  sorter.sort(src.get(), dst.get(), &creator, std::greater< int >());
  std::sort(vec.begin(), vec.end(), std::greater< int >());

  dst->rewind();

  for (int v: vec)
  {
    int val = 0;
    dst->read(val);
    BOOST_TEST(val == v);
    dst->next();
  }
}
