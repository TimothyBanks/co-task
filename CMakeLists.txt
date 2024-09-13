cmake_minimum_required (VERSION 3.8)
project (cotask)

set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_CXX_STANDARD LESS 20)
    message(FATAL_ERROR "C++20 is not supported by the compiler")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(TBB_TEST OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(libraries/oneTBB)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Boost 1.85.0)
find_package(Boost)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

add_library(cotask STATIC
    src/cotask_context.cpp 
    src/execution_guard.cpp 
    src/operation_context.cpp)

target_include_directories(cotask PUBLIC 
  ./include
  ./libraries/oneTBB/include)

  target_link_libraries(cotask tbb)

  add_subdirectory(tests)