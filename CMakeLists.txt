cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(MUNIN VERSION 0.3.8)

cmake_policy(VERSION 3.2)

if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

option(MUNIN_COVERAGE  "Build with code coverage options")
option(MUNIN_SANITIZE "Build using sanitizers" "")
option(MUNIN_WITH_TESTS "Build with tests" True)
message("Building Munin with config: ${CMAKE_BUILD_TYPE}")
message("Building Munin with code coverage: ${MUNIN_COVERAGE}")
message("Building Munin with sanitizers: ${MUNIN_SANITIZE}")
message("Building Munin with tests: ${MUNIN_WITH_TESTS}")

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message("Using Conan due to conanbuildinfo.cmake")
    set(MUNIN_USE_CONAN ON)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MUNIN_USE_CONAN)
    set(MUNIN_PUBLIC_LIBRARIES
        CONAN_PKG::terminalpp
        CONAN_PKG::nlohmann_json
        CONAN_PKG::boost
    )

    set(MUNIN_PRIVATE_LIBRARIES
    )

    set(MUNIN_TESTER_LIBRARIES
        munin
        CONAN_PKG::gtest
    )
else()
    # Munin requires at least Boost 1.69
    find_package(Boost 1.69.0 REQUIRED)

    # Munin requires the nlohmann_json package
    find_package(nlohmann_json 3.3.0 REQUIRED)

    # And of course, Munin is built atop the Terminal++ library, which
    # also requires fmt.
    find_package(fmt 5.3 REQUIRED)
    find_package(terminalpp 1.4.1 REQUIRED)

    # If we are building with tests, then we require the GTest library
    if (${MUNIN_WITH_TESTS})
        find_package(GTest REQUIRED CONFIG)
    endif()

    set(MUNIN_PUBLIC_LIBRARIES
        terminalpp
        nlohmann_json::nlohmann_json
        Boost::boost
    )

    set(MUNIN_PRIVATE_LIBRARIES
    )

    set(MUNIN_TESTER_LIBRARIES
        munin
        GTest::gtest
        GTest::gmock
        GTest::gmock_main
    )
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
        include/munin/horizontal_strip_layout.hpp
        include/munin/image.hpp
        include/munin/layout.hpp
        include/munin/list.hpp
        include/munin/null_layout.hpp
        include/munin/render_surface.hpp
        include/munin/solid_frame.hpp
        include/munin/status_bar.hpp
        include/munin/text_area.hpp
        include/munin/titled_frame.hpp
        include/munin/toggle_button.hpp
        include/munin/vertical_strip_layout.hpp
        include/munin/window.hpp
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
        src/horizontal_strip_layout.cpp
        src/image.cpp
        src/layout.cpp
        src/list.cpp
        src/null_layout.cpp
        src/render_surface.cpp
        src/solid_frame.cpp
        src/status_bar.cpp
        src/text_area.cpp
        src/titled_frame.cpp
        src/toggle_button.cpp
        src/vertical_strip_layout.cpp
        src/window.cpp
        src/viewport.cpp
    
        src/detail/adaptive_fill.cpp
        src/detail/algorithm.cpp
        src/detail/json_adaptors.cpp
)

target_link_libraries(munin
    PUBLIC
        ${MUNIN_PUBLIC_LIBRARIES}
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
        test/include/mock/animator.hpp
        test/include/mock/component.hpp
        test/include/mock/layout.hpp
        test/src/fill_canvas.cpp
        test/src/redraw.cpp
        test/src/mock/component.cpp
        test/src/mock/frame.cpp
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
        test/src/image/image_json_test.cpp
        test/src/image/image_test.cpp
        test/src/image/image_redraw_test.cpp
        test/src/image/new_image_test.cpp
        test/src/list/list_test.cpp
        test/src/null_layout/null_layout_test.cpp
        test/src/render_surface/render_surface_capabilities_test.cpp
        test/src/render_surface/render_surface_test.cpp
        test/src/solid_frame/solid_frame_json_test.cpp
        test/src/solid_frame/solid_frame_test.cpp
        test/src/status_bar/status_bar_test.cpp
        test/src/text_area/new_text_area_test.cpp
        test/src/text_area/text_area_test.cpp
        test/src/text_area/text_area_with_text_inserted_test.cpp
        test/src/text_area/text_area_test.hpp
        test/src/titled_frame/titled_frame_json_test.cpp
        test/src/titled_frame/titled_frame_test.cpp
        test/src/toggle_button/toggle_button_test.cpp
        test/src/toggle_button/toggle_button_json_test.cpp
        test/src/vertical_strip_layout/vertical_strip_layout_test.cpp
        test/src/viewport/viewport_test.hpp
        test/src/viewport/viewport_test.cpp
        test/src/viewport/viewport_cursor_test.cpp
        test/src/viewport/viewport_redraw_test.cpp
        test/src/viewport/viewport_size_test.cpp
        test/src/window/window_json_test.cpp
        test/src/window/window_test.cpp
        test/src/window/window_repaint_test.cpp
) 

target_include_directories(munin_tester
    PRIVATE
        ${PROJECT_SOURCE_DIR}/test/include
)

target_link_libraries(munin_tester
    ${MUNIN_TESTER_LIBRARIES}
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
