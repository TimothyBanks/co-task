add_executable(cotask_tests
  tests.cpp
)

target_link_libraries(cotask_tests cotask)
target_include_directories(cotask_tests PUBLIC ../include)