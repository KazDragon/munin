project(CHESS)

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)

    set(ROT13_SERVER_PRIVATE_LIBRARIES
        CONAN_PKG::terminalpp
        CONAN_PKG::munin
        CONAN_PKG::consolepp
        CONAN_PKG::boost
    )
else()
    find_package(terminalpp 3.0.0 REQUIRED)
    find_package(munin 0.7.1 REQUIRED)
    find_package(consolepp 0.1.1 REQUIRED)
    find_package(Boost REQUIRED)
    find_package(fmt REQUIRED)
    find_package(nlohmann_json REQUIRED)
    find_package(gsl-lite REQUIRED)
    find_package(Threads REQUIRED)

    set(CHESS_PRIVATE_LIBRARIES
        KazDragon::terminalpp
        KazDragon::munin
        KazDragon::consolepp
        Boost::boost
        Threads::Threads
    )
endif()

# The required C++ Standard for Server++ is C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (CHESS_PUBLIC_SOURCE_FILES
    src/chess.cpp
)

set (CHESS_PUBLIC_HEADER_FILES
)

add_executable(chess
    ${CHESS_PUBLIC_HEADER_FILES}
    ${CHESS_PUBLIC_SOURCE_FILES}
)

target_include_directories(chess
    PRIVATE 
        include
)

target_link_libraries(chess
    ${CHESS_PRIVATE_LIBRARIES}
)
