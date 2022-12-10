cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "-g")

project(IRTest)

# Google tests
find_package(GTest REQUIRED)
get_filename_component(TEST_SRC gtest/ ABSOLUTE)

# set includes
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(src)

#define test directory

include_directories(${GTEST_INCLUDE_DIRS})
add_executable(gtest
      gtest/gtest.cpp
      src/IR/src/Operand.cpp
      src/IR/src/BasicBlock.cpp
      src/IR/src/Inst.cpp
)
target_link_libraries(gtest ${GTEST_LIBRARIES} pthread)
