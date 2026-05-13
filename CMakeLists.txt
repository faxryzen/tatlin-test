cmake_minimum_required(VERSION 3.10)
project(TapeSort VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(LIB_SOURCES
  src/file-tape.cpp
)

add_executable(tape_sort
  src/main.cpp
  ${LIB_SOURCES}
)

find_package(Boost QUIET CONFIG COMPONENTS unit_test_framework)

if(NOT Boost_FOUND)
  cmake_policy(SET CMP0167 OLD)
  find_package(Boost REQUIRED COMPONENTS unit_test_framework)
endif()

add_executable(tape_test
  tests/test-default.cpp
  tests/test-file-tape.cpp
  tests/test-sorter.cpp
  ${LIB_SOURCES}
)

target_link_libraries(tape_test PRIVATE
  Boost::unit_test_framework
)

include_directories(${CMAKE_SOURCE_DIR}/common)

file(GLOB CONFIG_FILES ${CMAKE_SOURCE_DIR}/configs/*)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/configs)
foreach(CONFIG_FILE ${CONFIG_FILES})
  get_filename_component(CONFIG_NAME ${CONFIG_FILE} NAME)
  configure_file(
    ${CONFIG_FILE}
    ${CMAKE_BINARY_DIR}/configs/${CONFIG_NAME}
    COPYONLY
  )
endforeach()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/tmp)

enable_testing()

add_test(NAME tape_test COMMAND tape_test)
