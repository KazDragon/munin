project(HELLO_CONSOLE)

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

find_package(terminalpp 4.0.2 REQUIRED)
find_package(munin 0.7.1 REQUIRED)
find_package(consolepp 0.1.1 REQUIRED)
find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (HELLO_CONSOLE_PUBLIC_SOURCE_FILES
    src/hello_console.cpp
)

set (HELLO_CONSOLE_PUBLIC_HEADER_FILES
)

add_executable(hello_console
    ${HELLO_CONSOLE_PUBLIC_HEADER_FILES}
    ${HELLO_CONSOLE_PUBLIC_SOURCE_FILES}
)

target_include_directories(hello_console
    PRIVATE 
        include
)

target_link_libraries(hello_console
    PRIVATE
        KazDragon::terminalpp
        KazDragon::munin
        KazDragon::consolepp
        Boost::boost
        Threads::Threads
)
