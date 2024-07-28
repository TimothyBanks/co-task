cmake_minimum_required (VERSION 3.8)
project (cotask)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_AR "/usr/bin/llvm-ar")
set(CMAKE_LINKER "/usr/bin/llvm-ld")
set(CMAKE_NM "/usr/bin/llvm-nm")
set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
set(CMAKE_RANLIB "/usr/bin/llvm-ranlib")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.85.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(TBB_TEST OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(libraries/oneTBB)

add_library(cotask STATIC  
    src/awaitable.cpp 
    src/cotask_context.cpp 
    src/execution_guard.cpp 
    src/operation_context.cpp 
    src/task.cpp 
    src/this_thread.cpp)

target_include_directories(cotask PUBLIC 
  ./include
  ./libraries/oneTBB/include)