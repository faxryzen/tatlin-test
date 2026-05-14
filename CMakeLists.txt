cmake_minimum_required(VERSION 3.10)
project(TapeSort VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/common)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

add_executable(tape_sort
  src/main.cpp
  ${LIB_SOURCES}
)

option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
  find_package(Boost QUIET COMPONENTS unit_test_framework)

  if(Boost_FOUND)
    file(GLOB TEST_SOURCES "tests/*.cpp")
    add_executable(tape_test
      ${TEST_SOURCES}
      ${LIB_SOURCES}
    )
    target_link_libraries(tape_test PRIVATE Boost::unit_test_framework)

    enable_testing()
    add_test(NAME tape_test COMMAND tape_test)
  else()
    message(STATUS "Boost not found - tests disabled")
  endif()
endif()

file(GLOB CONFIG_FILES "${CMAKE_SOURCE_DIR}/configs/*")
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