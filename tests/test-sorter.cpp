#include <boost/test/unit_test.hpp>

#include "file-tape.hpp"
#include "sorter.hpp"

BOOST_AUTO_TEST_CASE(sort1)
{
  using namespace savintsev;

  TempFileTapeCreator creator;
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

BOOST_AUTO_TEST_CASE(sort2)
{
  using namespace savintsev;

  TempFileTapeCreator creator;
  std::unique_ptr< Tape > src = creator.create();
  std::unique_ptr< Tape > dst = creator.create();

  std::vector< int > vec{10, 432, 8, 43, 64, 87, 2, 4, 65, 23, 76, 1, 13, 87, 134, 87, 42, 98, 24,  64, 13, 876, 9, 67843, 3, 643, 6, 346};
  for (int v: vec)
  {
    src->write(v);
    src->next();
  }
  src->rewind();

  TapeSorter sorter;
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
