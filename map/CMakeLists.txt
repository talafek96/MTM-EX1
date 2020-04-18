cmake_minimum_required(VERSION 3.0.0)
project(TestCode VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic-errors -DNDEBUG")
add_library(map map.c)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)