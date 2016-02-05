cmake_minimum_required(VERSION 3.2)
project(short_alloc)


###########
# Debug or Release
###########
if (NOT CMAKE_BUILD_TYPE)
  ## set default to Debug
  set(CMAKE_BUILD_TYPE Debug)  # override with -DCMAKE_BUILD_TYPE=Release
  message("==> CMAKE_BUILD_TYPE empty. Changing it to Debug.")
else()
  message("==> CMAKE_BUILD_TYPE == ${CMAKE_BUILD_TYPE}.")
endif()

## https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_macros.html
## During Debug, use debug version of libstdc++ (asserts on access to invalid iterators, etc!)
if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
endif ()

###########
# Warnings
###########
if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-unused-variable")        # suppress
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations") # suppress
endif ()



##############
# C++ Standard
##############
# c++14
if (NOT    CMAKE_VERSION VERSION_LESS 3.1)
  set(CMAKE_CXX_STANDARD   14)
  set(CMAKE_CXX_EXTENSIONS OFF)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()


add_executable(go main.cpp)
