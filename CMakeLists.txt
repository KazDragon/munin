cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
cmake_policy(VERSION 3.13)

option(MUNIN_WITH_CONSOLEPP "Build using Console++" False)
option(MUNIN_COVERAGE  "Build with code coverage options")
option(MUNIN_SANITIZE "Build using sanitizers" "")
option(MUNIN_WITH_TESTS "Build with tests" True)
option(MUNIN_DOC_ONLY "Build only documentation" False)

message("Building Munin with Console++: ${MUNIN_WITH_CONSOLEPP}")
message("Building Munin with config: ${CMAKE_BUILD_TYPE}")
message("Building Munin with code coverage: ${MUNIN_COVERAGE}")
message("Building Munin with sanitizers: ${MUNIN_SANITIZE}")
message("Building Munin with tests: ${MUNIN_WITH_TESTS}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

if (NOT MUNIN_VERSION)
    include(function-git_version)
    git_version(MUNIN)
endif()

message(STATUS "Munin Version: ${MUNIN_VERSION}")
project(MUNIN VERSION ${MUNIN_VERSION})

# For producing automatically-generated documentation, we use Doxygen.
find_package(Doxygen)

# Add a rule for generating documentation
if (DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        @ONLY
    )

    add_custom_target(munin_doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT
            "Generate API documentation with Doxygen" VERBATIM
    )
endif()

if (MUNIN_DOC_ONLY)
    return()
endif()

# Munin requires at least Boost 1.69
find_package(Boost 1.69.0 REQUIRED)

# Munin requires the nlohmann_json package
find_package(nlohmann_json 3.3.0 CONFIG REQUIRED)

# And of course, Munin is built atop the Terminal++ library, which
# also requires fmt
find_package(fmt 5.3 CONFIG REQUIRED)

if (NOT TARGET KazDragon::terminalpp)
    find_package(terminalpp 4.0.0 CONFIG REQUIRED)
endif()

# If we are building with Console++, then require that library
if (MUNIN_WITH_CONSOLEPP)
    if (NOT TARGET KazDragon::consolepp)
        find_package(consolepp 0.1.1 CONFIG REQUIRED)
    endif()
    
    set(MUNIN_PRIVATE_LIBRARIES
        ${MUNIN_PRIVATE_LIBRARIES}
        KazDragon::consolepp
    )
endif()

# If we are building with tests, then we require the GTest library
if (MUNIN_WITH_TESTS)
    find_package(GTest CONFIG REQUIRED)
endif()

# When building shared objects, etc., we only want to export certain symbols.
# Therefore, we need to generate headers suitable for declaring which symbols
# should be included.
include(GenerateExportHeader)

# For producing automatically-generated documentation, we use Doxygen
find_package(Doxygen)

if (MUNIN_SANITIZE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${MUNIN_SANITIZE}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${MUNIN_SANITIZE}")
endif()

if (MUNIN_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

add_library(munin)
add_library(KazDragon::munin ALIAS munin)

target_sources(munin
    PRIVATE
        include/munin/animator.hpp
        include/munin/background_animator.hpp
        include/munin/basic_component.hpp
        include/munin/brush.hpp
        include/munin/button.hpp
        include/munin/component.hpp
        include/munin/composite_component.hpp
        include/munin/container.hpp
        include/munin/edit.hpp
        include/munin/filled_box.hpp
        include/munin/framed_component.hpp
        include/munin/grid_layout.hpp
        include/munin/horizontal_scrollbar.hpp
        include/munin/horizontal_strip_layout.hpp
        include/munin/image.hpp
        include/munin/layout.hpp
        include/munin/list.hpp
        include/munin/null_layout.hpp
        include/munin/render_surface.hpp
        include/munin/scroll_pane.hpp
        include/munin/scroll_frame.hpp
        include/munin/solid_frame.hpp
        include/munin/status_bar.hpp
        include/munin/text_area.hpp
        include/munin/titled_frame.hpp
        include/munin/toggle_button.hpp
        include/munin/vertical_strip_layout.hpp
        include/munin/window.hpp
        include/munin/vertical_scrollbar.hpp
        include/munin/version.hpp
        include/munin/view.hpp
        include/munin/viewport.hpp
    
        include/munin/detail/adaptive_fill.hpp
        include/munin/detail/algorithm.hpp
        include/munin/detail/json_adaptors.hpp
    
        src/aligned_layout.cpp
        src/animator.cpp
        src/background_animator.cpp
        src/basic_component.cpp
        src/brush.cpp
        src/button.cpp
        src/compass_layout.cpp
        src/component.cpp
        src/composite_component.cpp
        src/container.cpp
        src/edit.cpp
        src/filled_box.cpp
        src/frame.cpp
        src/framed_component.cpp
        src/grid_layout.cpp
        src/horizontal_scrollbar.cpp
        src/horizontal_strip_layout.cpp
        src/image.cpp
        src/layout.cpp
        src/list.cpp
        src/null_layout.cpp
        src/render_surface.cpp
        src/scroll_pane.cpp
        src/scroll_frame.cpp
        src/solid_frame.cpp
        src/status_bar.cpp
        src/text_area.cpp
        src/titled_frame.cpp
        src/toggle_button.cpp
        src/vertical_strip_layout.cpp
        src/window.cpp
        src/vertical_scrollbar.cpp
        src/viewport.cpp
    
        src/detail/adaptive_fill.cpp
        src/detail/algorithm.cpp
        src/detail/json_adaptors.cpp
)

target_compile_options(munin
    PRIVATE
        # Do not generate warning C4251 (member needs dll linkage) on MSVC
        $<$<CXX_COMPILER_ID:MSVC>:/wd4251>
        # Do not generate warning C4275 (type needs dll linkage) on MSVC
        $<$<CXX_COMPILER_ID:MSVC>:/wd4275>
        # Any warning on MSVC is an error
        $<$<CXX_COMPILER_ID:MSVC>:/WX>

        # Add warnings on g++ and Clang
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Werror -Wno-sign-compare -Wno-tautological-compare -Wno-unused-parameter>
)

if (MUNIN_WITH_CONSOLEPP)
    target_sources(munin
        PRIVATE
            include/munin/console_application.hpp
            src/console_application.cpp
    )
endif()

target_link_libraries(munin
    PUBLIC
        KazDragon::terminalpp
        nlohmann_json::nlohmann_json
        Boost::boost
    PRIVATE
        ${MUNIN_PRIVATE_LIBRARIES}
)

target_include_directories(munin
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/munin-${MUNIN_VERSION}>
)

set_target_properties(munin
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VERSION ${MUNIN_VERSION}
        SOVERSION ${MUNIN_VERSION}
        DEBUG_POSTFIX "d"
)        

target_compile_options(munin
    PRIVATE
        # Do not generate warning C4251 (member needs dll linkage) on MSVC
        $<$<CXX_COMPILER_ID:MSVC>:/wd4251>
        # No warnings are generated on MSVC
        $<$<CXX_COMPILER_ID:MSVC>:/WX>
)

# The required C++ Standard for Telnet++ is C++20.
target_compile_features(munin
    PUBLIC 
        cxx_std_20
)

generate_export_header(munin
    EXPORT_FILE_NAME
        "${PROJECT_SOURCE_DIR}/include/munin/export.hpp"
)        

if (MUNIN_WITH_TESTS)
enable_testing()

add_executable(munin_tester)

target_sources(munin_tester
    PRIVATE
        test/src/container/container_test.hpp
        test/src/window/window_test.hpp
        test/include/fill_canvas.hpp
        test/include/redraw.hpp
        test/include/fake/channel.hpp
        test/include/mock/animator.hpp
        test/include/mock/component.hpp
        test/include/mock/layout.hpp
        test/src/assert_similar.cpp
        test/src/fill_canvas.cpp
        test/src/redraw.cpp
        test/src/mock/animator.cpp
        test/src/mock/component.cpp
        test/src/mock/frame.cpp
        test/src/mock/layout.cpp
        test/src/algorithm/algorithm_test.cpp
        test/src/aligned_layout/aligned_layout_test.cpp
        test/src/animator/animator_test.cpp
        test/src/basic_component/new_basic_component_test.cpp
        test/src/basic_component/unfocused_basic_component_test.cpp
        test/src/basic_component/focused_basic_component_test.cpp
        test/src/basic_component/basic_component_test.cpp
        test/src/brush/brush_test.cpp
        test/src/brush/brush_json_test.cpp
        test/src/brush/brush_redraw_test.cpp
        test/src/brush/new_brush_test.cpp
        test/src/button/button_test.cpp
        test/src/button/button_json_test.cpp
        test/src/compass_layout/compass_layout_test.cpp
        test/src/composite_component/composite_component_test.cpp
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
        test/src/edit/edit_test.cpp
        test/src/edit/edit_mouse_test.cpp
        test/src/edit/edit_with_content_test.cpp
        test/src/filled_box/new_filled_box_test.cpp
        test/src/filled_box/filled_box_test.cpp
        test/src/filled_box/functional_filled_box_test.cpp
        test/src/framed_component/framed_component_focus_test.cpp
        test/src/framed_component/framed_component_highlight_test.cpp
        test/src/framed_component/framed_component_json_test.cpp
        test/src/framed_component/framed_component_test.cpp
        test/src/framed_component/framed_component_mouse_test.cpp
        test/src/grid_layout/grid_layout_test.cpp
        test/src/horizontal_strip_layout/horizontal_strip_layout_test.cpp
        test/src/horizontal_scrollbar/horizontal_scrollbar_test.cpp
        test/src/image/image_json_test.cpp
        test/src/image/image_test.cpp
        test/src/image/image_redraw_test.cpp
        test/src/image/new_image_test.cpp
        test/src/list/list_test.cpp
        test/src/null_layout/null_layout_test.cpp
        test/src/render_surface/render_surface_capabilities_test.cpp
        test/src/render_surface/render_surface_test.cpp
        test/src/scroll_frame/scroll_frame_test.cpp
        test/src/scroll_pane/scroll_pane_test.cpp
        test/src/solid_frame/solid_frame_json_test.cpp
        test/src/solid_frame/solid_frame_test.cpp
        test/src/status_bar/status_bar_test.cpp
        test/src/text_area/new_text_area_test.cpp
        test/src/text_area/text_area_test.cpp
        test/src/text_area/text_area_with_text_inserted_test.cpp
        test/src/text_area/text_area_typing_test.cpp
        test/src/text_area/text_area_test.hpp
        test/src/titled_frame/titled_frame_json_test.cpp
        test/src/titled_frame/titled_frame_test.cpp
        test/src/toggle_button/toggle_button_test.cpp
        test/src/toggle_button/toggle_button_json_test.cpp
        test/src/vertical_scrollbar/vertical_scrollbar_test.cpp
        test/src/vertical_strip_layout/vertical_strip_layout_test.cpp
        test/src/viewport/viewport_test.hpp
        test/src/viewport/viewport_test.cpp
        test/src/viewport/viewport_resize_strategy_test.cpp
        test/src/viewport/viewport_cursor_test.cpp
        test/src/viewport/viewport_keypress_test.cpp
        test/src/viewport/viewport_redraw_test.cpp
        test/src/viewport/viewport_size_test.cpp
        test/src/window/window_json_test.cpp
        test/src/window/window_test.cpp
        test/src/window/window_render_capabilities_test.cpp
        test/src/window/window_repaint_test.cpp
) 

target_include_directories(munin_tester
    PRIVATE
        ${PROJECT_SOURCE_DIR}/test/include
)

target_link_libraries(munin_tester
    PRIVATE
        KazDragon::munin
        GTest::gmock_main
)

target_compile_options(munin_tester
    PRIVATE
        # Do not generate warning C4251 (member needs dll linkage) on MSVC
        $<$<CXX_COMPILER_ID:MSVC>:/wd4251>
        # No warnings are generated on MSVC
        $<$<CXX_COMPILER_ID:MSVC>:/WX>
)

add_test(munin_test munin_tester)

# For an MSVC build where terminalpp is a shared library, ensure that
# the DLL gets copied into the executable directory so that the tests can run.
if (MSVC)
    get_target_property(terminalpp_type KazDragon::terminalpp TYPE)

    if (terminalpp_type STREQUAL "SHARED_LIBRARY")
        add_custom_command(TARGET munin_tester POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:KazDragon::terminalpp>
            $<TARGET_FILE_DIR:munin_tester>
       )
    endif()
endif()

endif()

configure_file(
    ${PROJECT_SOURCE_DIR}/include/munin/version.hpp.in
    ${PROJECT_SOURCE_DIR}/include/munin/version.hpp
    @ONLY)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/munin-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/munin-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/munin
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/munin-config-version.cmake"
    VERSION
        "${MUNIN_VERSION}"
    COMPATIBILITY AnyNewerVersion
)

export(
    TARGETS 
        munin
    NAMESPACE
        KazDragon::
    FILE ${PROJECT_BINARY_DIR}/munin-targets.cmake
)

install(
    TARGETS
        munin
    EXPORT
        munin-targets
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/munin-${MUNIN_VERSION}
    ARCHIVE DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/munin-${MUNIN_VERSION}
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_BINDIR}/munin-${MUNIN_VERSION}
)

install(
    EXPORT
        munin-targets
    NAMESPACE
        KazDragon::
    DESTINATION
        ${CMAKE_INSTALL_DATADIR}/munin-${MUNIN_VERSION}
)

install(
    DIRECTORY
        include/
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/munin-${MUNIN_VERSION}
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/munin-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/munin-config-version.cmake"
    DESTINATION
        ${CMAKE_INSTALL_DATADIR}/munin-${MUNIN_VERSION}
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
