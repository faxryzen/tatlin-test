cmake_minimum_required(VERSION 3.10)
project(TapeSort VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
  message(STATUS "nlohmann_json not found, using FetchContent")
  include(FetchContent)
  FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
  )
  FetchContent_MakeAvailable(json)
endif()

set(LIB_SOURCES
  src/file-tape.cpp
)

add_executable(tape_sort
  src/main.cpp
  ${LIB_SOURCES}
)

if(nlohmann_json_FOUND)
  target_link_libraries(tape_sort PRIVATE nlohmann_json::nlohmann_json)
else()
  target_link_libraries(tape_sort PRIVATE nlohmann_json)
endif()

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

if(nlohmann_json_FOUND)
  target_link_libraries(tape_test PRIVATE nlohmann_json::nlohmann_json)
else()
  target_link_libraries(tape_test PRIVATE nlohmann_json)
endif()

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
