cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(munin)
cmake_policy(VERSION 3.2)

if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Some Boost components are required for this library
include(FindBoost)
find_package(Boost
    REQUIRED
    COMPONENTS
        system)

# Terminal++ is required for this library.
find_package(Terminalpp REQUIRED)

# Due to the strange requirements for Google Test, it is assumed to have been
# built in the "gtest" directory.  For this, it is required to run the moral
# equivalent of the following script before building Munin
#
#  mkdir gtest
#  cd gtest
#  cmake /usr/src/gtest && make
#  export GTEST_ROOT=$PWD
#  cd ..
find_package(GTest)

if (GTEST_FOUND)
    find_package(GMock REQUIRED)
    
    # For GTest, it is necessary to link with the appropriate local threading
    # library.
    find_package(Threads)

    enable_testing()
endif()

# When building shared objects, etc., we only want to export certain symbols.
# Therefore, we need to generate headers suitable for declaring which symbols
# should be included.
include(GenerateExportHeader)

if (MUNIN_SANITIZE)
    list(APPEND CMAKE_CXX_FLAGS "-fsanitize=${MUNIN_SANITIZE}")
    list(APPEND CMAKE_EXE_LINKER_FLAGS "-fsanitize=${MUNIN_SANITIZE}")
endif()

if (MUNIN_COVERAGE)
    list(APPEND CMAKE_CXX_FLAGS "--coverage -g -O0")
    list(APPEND CMAKE_EXE_LINKER_FLAGS "--coverage")
endif()

set (MUNIN_VERSION 0.0.1)

set (munin_SOURCES
    src/algorithm.cpp
    src/basic_component.cpp
    src/basic_container.cpp
    src/component.cpp
    src/context.cpp
    src/filled_box.cpp
    src/horizontal_strip_layout.cpp
    src/layout.cpp
    src/window.cpp
)

add_library(munin ${munin_SOURCES})

generate_export_header(munin
    EXPORT_FILE_NAME
        "${PROJECT_SOURCE_DIR}/include/munin/export.hpp"
)

set_target_properties(munin
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VERSION ${MUNIN_VERSION}
        SOVERSION ${MUNIN_VERSION}
)

target_include_directories(munin
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        "${Boost_INCLUDE_DIRS}"
)

target_link_libraries(munin
    PUBLIC
        terminalpp
        ${Boost_SYSTEM_LIBRARY}
)

target_compile_features(munin
    PRIVATE
        cxx_auto_type
        cxx_strong_enums
        cxx_generic_lambdas
)

if (GTEST_FOUND)
    enable_testing()

    set (munin_test_SOURCES
        test/algorithm_test.cpp
        test/basic_component/new_basic_component_test.cpp
        test/basic_component/enabled_basic_component_test.cpp
        test/basic_component/disabled_basic_component_test.cpp
        test/basic_component/unfocused_basic_component_test.cpp
        test/basic_component/focused_basic_component_test.cpp
        test/basic_component/basic_component_test.cpp
        test/filled_box/new_filled_box_test.cpp
        test/filled_box/filled_box_test.cpp
        test/horizontal_strip_layout_test.cpp
    )

    add_executable(munin_tester
        ${munin_test_SOURCES}
    )

    target_include_directories(munin_tester
        PRIVATE
            ${GTEST_INCLUDE_DIR}
    )
    
    target_link_libraries(munin_tester
        munin
        terminalpp-core
        ${GMOCK_LIBRARY}
        ${GTEST_LIBRARY}
        ${GTEST_MAIN_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
    )

    add_test(munin_test munin_tester)
endif()
