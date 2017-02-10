cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(munin)
cmake_policy(VERSION 3.2)

if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(FindBoost)
find_package(Boost REQUIRED)

# When building shared objects, etc., we only want to export certain symbols.
# Therefore, we need to generate headers suitable for declaring which symbols
# should be included.
include(GenerateExportHeader)

# Due to the strange requirements for Google Test, it is assumed to have been
# built in the "gtest" directory.  For this, it is required to run the moral
# equivalent of the following script before building Telnet++:
#
#  mkdir gtest
#  cd gtest
#  cmake /usr/src/gtest && make
#  export GTEST_ROOT=$PWD
#  cd ..
find_package(GTest)

# For GTest, it is necessary to link with the appropriate local threading 
# library.
find_package(Threads)

if (GTEST_FOUND)
    enable_testing()
endif()

set (munin_SOURCES
    src/munin/dummy.cpp
)

add_library(munin ${munin_SOURCES})

target_include_directories(munin
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(munin
    PUBLIC
        terminalpp
)

target_compile_features(munin
    PRIVATE 
        cxx_auto_type
        cxx_strong_enums
        cxx_generic_lambdas
)

