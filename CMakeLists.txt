cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(MUNIN VERSION 0.2)

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
find_package(Terminalpp 1.2.7 REQUIRED)

# Due to the strange requirements for Google Mock, it is assumed to have been
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

# For producing automatically-generated documentation, we use Doxygen
find_package(Doxygen)

# When building shared objects, etc., we only want to export certain symbols.
# Therefore, we need to generate headers suitable for declaring which symbols
# should be included.
include(GenerateExportHeader)

# Add nlohmann_json for easy JSON support.
option(BuildTests "JSON Unit Tests" OFF)
add_subdirectory(json)

if (MUNIN_SANITIZE)
    list(APPEND CMAKE_CXX_FLAGS "-fsanitize=${MUNIN_SANITIZE}")
    list(APPEND CMAKE_EXE_LINKER_FLAGS "-fsanitize=${MUNIN_SANITIZE}")
endif()

if (MUNIN_COVERAGE)
    list(APPEND CMAKE_CXX_FLAGS "--coverage -g -O0")
    list(APPEND CMAKE_EXE_LINKER_FLAGS "--coverage")
endif()

set (MUNIN_SOURCE_FILES
    src/aligned_layout.cpp
    src/basic_component.cpp
    src/brush.cpp
    src/compass_layout.cpp
    src/component.cpp
    src/composite_component.cpp
    src/container.cpp
    src/filled_box.cpp
    src/framed_component.cpp
    src/grid_layout.cpp
    src/horizontal_strip_layout.cpp
    src/image.cpp
    src/layout.cpp
    src/null_layout.cpp
    src/render_surface.cpp
    src/solid_frame.cpp
    src/titled_frame.cpp
    src/vertical_strip_layout.cpp
    src/window.cpp

    src/detail/adaptive_fill.cpp
    src/detail/algorithm.cpp
    src/detail/json_adaptors.cpp
)

set (MUNIN_INCLUDE_FILES
    include/munin/basic_component.hpp
    include/munin/brush.hpp
    include/munin/component.hpp
    include/munin/composite_component.hpp
    include/munin/container.hpp
    include/munin/filled_box.hpp
    include/munin/framed_component.hpp
    include/munin/grid_layout.hpp
    include/munin/horizontal_strip_layout.hpp
    include/munin/image.hpp
    include/munin/layout.hpp
    include/munin/null_layout.hpp
    include/munin/rectangle.hpp
    include/munin/render_surface.hpp
    include/munin/solid_frame.hpp
    include/munin/titled_frame.hpp
    include/munin/vertical_strip_layout.hpp
    include/munin/window.hpp
    include/munin/view.hpp

    include/munin/detail/adaptive_fill.hpp
    include/munin/detail/algorithm.hpp
    include/munin/detail/json_adaptors.hpp
)

add_library(munin
    ${MUNIN_SOURCE_FILES}
    ${MUNIN_INCLUDE_FILES}
)

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
        $<INSTALL_INTERFACE:include/munin-${MUNIN_VERSION}>
        "${Boost_INCLUDE_DIRS}"
)

target_link_libraries(munin
    PUBLIC
        terminalpp
        nlohmann_json
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

    set (MUNIN_TEST_SOURCE_FILES
        test/src/redraw.cpp
        test/src/mock/component.cpp
        test/src/algorithm/algorithm_test.cpp
        test/src/aligned_layout/aligned_layout_test.cpp
        test/src/basic_component/new_basic_component_test.cpp
        test/src/basic_component/unfocused_basic_component_test.cpp
        test/src/basic_component/focused_basic_component_test.cpp
        test/src/basic_component/basic_component_test.cpp
        test/src/brush/brush_test.cpp
        test/src/brush/brush_json_test.cpp
        test/src/brush/brush_redraw_test.cpp
        test/src/brush/new_brush_test.cpp
        test/src/compass_layout/compass_layout_test.cpp
        test/src/container/container_test.cpp
        test/src/container/container_cursor_test.cpp
        test/src/container/container_draw_test.cpp
        test/src/container/container_event_test.cpp
        test/src/container/container_focus_test.cpp
        test/src/container/container_focus_next_test.cpp
        test/src/container/container_focus_previous_test.cpp
        test/src/container/container_json_test.cpp
        test/src/container/container_redraw_test.cpp
        test/src/container/container_subcomponent_focus_test.cpp
        test/src/container/container_layout_test.cpp
        test/src/filled_box/new_filled_box_test.cpp
        test/src/filled_box/filled_box_test.cpp
        test/src/filled_box/functional_filled_box_test.cpp
        test/src/framed_component/framed_component_focus_test.cpp
        test/src/framed_component/framed_component_json_test.cpp
        test/src/framed_component/framed_component_test.cpp
        test/src/framed_component/framed_component_mouse_test.cpp
        test/src/grid_layout/grid_layout_test.cpp
        test/src/horizontal_strip_layout/horizontal_strip_layout_test.cpp
        test/src/image/image_json_test.cpp
        test/src/image/image_test.cpp
        test/src/image/image_redraw_test.cpp
        test/src/image/new_image_test.cpp
        test/src/null_layout/null_layout_test.cpp
        test/src/render_surface/render_surface_capabilities_test.cpp
        test/src/render_surface/render_surface_test.cpp
        test/src/solid_frame/solid_frame_json_test.cpp
        test/src/solid_frame/solid_frame_test.cpp
        test/src/titled_frame/titled_frame_json_test.cpp
        test/src/titled_frame/titled_frame_test.cpp
        test/src/vertical_strip_layout/vertical_strip_layout_test.cpp
        test/src/window/window_json_test.cpp
        test/src/window/window_test.cpp
        test/src/window/window_repaint_test.cpp
    )

    set (MUNIN_TEST_INCLUDE_FILES
        test/src/container/container_test.hpp
        test/src/window/window_test.hpp
        test/include/redraw.hpp
        test/include/mock/component.hpp
        test/include/mock/layout.hpp
        test/include/mock/terminal.hpp
    )

    add_executable(munin_tester
        ${MUNIN_TEST_SOURCE_FILES}
        ${MUNIN_TEST_INCLUDE_FILES}
    )

    target_include_directories(munin_tester
        PRIVATE
            ${PROJECT_SOURCE_DIR}/test/include
            ${GTEST_INCLUDE_DIR}
    )

    target_link_libraries(munin_tester
        munin
        terminalpp-core
        ${GMOCK_LIBRARY}
        ${GTEST_LIBRARY}
        ${GMOCK_MAIN_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
    )

    add_test(munin_test munin_tester)
endif()

if (DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        @ONLY
    )

    add_custom_target("${CMAKE_PROJECT_NAME}_doc"
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT
            "Generate API documentation with Doxygen" VERBATIM
    )
endif()

install(
    TARGETS
        munin
    EXPORT
        munin-config
    ARCHIVE DESTINATION
        lib/munin-${MUNIN_VERSION}
    LIBRARY DESTINATION
        lib/munin-${MUNIN_VERSION}
)

install(
    DIRECTORY
        include/
    DESTINATION
        include/munin-${MUNIN_VERSION}
)

export(
    EXPORT
        munin-config
    FILE
        "${CMAKE_CURRENT_BINARY_DIR}/munin-config.cmake"
)

install(
    EXPORT
        munin-config
    DESTINATION
        lib/munin-${MUNIN_VERSION}
)


include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/munin-config-version.cmake"
    VERSION
        "${MUNIN_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/munin-config-version.cmake"
    DESTINATION
        lib/munin-${MUNIN_VERSION}
)

# Add customizations for packaging
set(CPACK_PACKAGE_NAME "Munin")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Munin")
set(CPACK_PACKAGE_VENDOR "Matthew Chaplain")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR ${MUNIN_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${MUNIN_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${MUNIN_VERSION_PATCH})
include(CPack)
