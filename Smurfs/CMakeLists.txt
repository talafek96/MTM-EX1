cmake_minimum_required(VERSION 3.0.0)
project(election VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic-errors -Werror -DNDEBUG") # -Werror
add_library(election election.c)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
