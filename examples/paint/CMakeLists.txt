project(PAINT)

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(VERSION 3.2)

if (NOT TARGET KazDragon::terminalpp)
    find_package(terminalpp 4.0.0 REQUIRED)
endif()

if (NOT TARGET KazDragon::munin)
    find_package(munin 0.7.1 REQUIRED)
endif()

if (NOT TARGET KazDragon::consolepp)
    find_package(consolepp 0.1.1 REQUIRED)
endif()

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PAINT_PUBLIC_SOURCE_FILES
    src/paint_canvas.cpp
    src/main.cpp
)

set(PAINT_PUBLIC_HEADER_FILES
    include/paint/paint_canvas.hpp
    include/paint/paint_model.hpp
)

add_executable(paint
    ${PAINT_PUBLIC_HEADER_FILES}
    ${PAINT_PUBLIC_SOURCE_FILES}
)

target_include_directories(paint
    PRIVATE
        include
)

target_link_libraries(paint
    PRIVATE
        KazDragon::terminalpp
        KazDragon::munin
        KazDragon::consolepp
        Boost::boost
        Threads::Threads
)

if (MUNIN_WITH_TESTS)
    add_executable(paint_tester
        ${PAINT_PUBLIC_HEADER_FILES}
        src/paint_canvas.cpp
        test/paint_model_test.cpp
        test/paint_canvas_test.cpp
    )

    target_include_directories(paint_tester
        PRIVATE
            include
    )

    target_link_libraries(paint_tester
        PRIVATE
            KazDragon::terminalpp
            KazDragon::munin
            GTest::gmock_main
    )

    add_test(paint_test paint_tester)
endif()
