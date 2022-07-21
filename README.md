# Munin

[![Documentation](https://img.shields.io/badge/code-documented-brightgreen.svg?style=flat)](https://kazdragon.github.io/munin/) 
[![License](https://img.shields.io/github/license/KazDragon/munin.svg)](https://en.wikipedia.org/wiki/MIT_License)
[![MSVC Build status](https://ci.appveyor.com/api/projects/status/p7wcwnd4xapnetkl/branch/master?svg=true)](https://ci.appveyor.com/project/KazDragon/munin/branch/master)
[![Linux Build status](https://github.com/KazDragon/munin/workflows/build/badge.svg)](https://github.com/KazDragon/munin/actions?query=event%3Apush) 
[![Coverage Status](https://coveralls.io/repos/github/KazDragon/munin/badge.svg?branch=master)](https://coveralls.io/github/KazDragon/munin?branch=master)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/cbf22a847dc040b1a9dee8be3eda00d3)](https://www.codacy.com/gh/KazDragon/munin/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=KazDragon/munin&amp;utm_campaign=Badge_Grade)

Munin is an enhancement to the Terminal++ library and provides a set of textual, component-based UI widgets.  Windows are composed using layouts that automatically size and position their subcomponents according to the rules you specify:

```c++
view(
    make_compass_layout(),
    view(
        // Lay out the chess board and move lists next to each other
        make_grid_layout({2, 1}), // two components horizontally
        view(
            make_compass_layout(),
            chess_board,
            compass_layout::heading::centre,
            make_fill("\\[6\\U2502"_ete), // cyan vertical bar
            compass_layout::heading::east),
        view(
            make_grid_layout({1, 2}), // two components vertically
            make_scroll_pane(white_moves_history),
            make_scroll_pane(black_moves_history)))),
    compass_layout::heading::centre,
    // Lay out a view for a button below the chess board and move lists
    view(
        make_compass_layout(),
        make_fill("\\[6\\U2500"_ete), // cyan horizontal bar
        compass_layout::heading::north,
        make_fill(' '),
        compass_layout::heading::centre,
        ok_button,
        compass_layout::heading::east),
    compass_layout::heading::south);
```

![Example layout of chess board](https://github.com/KazDragon/munin/blob/master/examples/chess/img/chess.png)

See [The Chess Example](examples/chess) for an example of how to tie this into a console application.

# Requirements

Munin requires a C++17 compiler and the following libraries:
  * Boost (At least version 1.69.0)
  * nlohmann_json (At least version 3.3.0)
  * [Terminal++](https://github.com/KazDragon/terminalpp) (At least version 2.4.0)
  * (for Terminal++) libfmt (At least version 5.3) and gsl-lite (at least version 0.38)
  * Optionally, [Console++](https://github.com/KazDragon/consolepp) (At least version 0.1.0)
  * (for testing only) Google Test and Google Mock

# Installation - CMake 

Munin can be installed from source using CMake.  This requires Boost, Terminal++ and any other dependencies to have been installed beforehand, using their own instructions, or for the call to `cmake --configure` to be adjusted appropriately (e.g. `-DBOOST_ROOT=...` or `-DGtest_DIR=...`).  If you do not wish to install into a system directory, and thus avoid the use of sudo, you can also pass `-DCMAKE_INSTALL_PREFIX=...` into the `cmake --configure` call.

    git clone https://github.com/KazDragon/munin.git && cd munin
    mkdir build && cd build
    cmake --configure -DCMAKE_BUILD_TYPE=Release ..
    cmake --build .
    sudo cmake --install .

# Installation - Conan

You can also use [the Conan Package Manager](https://conan.io/) to install Munin and its dependencies.

See [the TextRay project](https://github.com/KazDragon/textray) for a project that describes this kind of setup.

# Components:

## Fundamentals
1. [x] Component - the base class of all UI components
2. [x] Container/Layout - a component that can contain and arrange other components
3. [x] A sampling of common layouts 
 * [x] Null Layout - a layout that doesn't.
 * [x] Vertical Strip Layout - a layout that arranges components in vertical strips
 * [x] Horizontal Strip Layout - a layout that arranges components in horizontal strips
 * [x] Grid Layout - a layout that arranges components in equally-sized cells.
 * [x] Aligned Layout - a layout that arranges components with horizontal and vertical alignment (e.g. top-left, right-center)
 * [x] Compass Layout - a layout that arranges components according to compass points (e.g. North, South, Centre)
4. [x] A sampling of common components
 * [x] Filled Box - a component that simply paints a single repeated character
 * [x] Brush - a component that paints a repeated character pattern.
 * [x] Image - a component that paints a static "image" once.
5. [x] Window - a top-level object that contains and draws components onto a canvas.

 ## Frames
 Implement the ability to frame components
 1. [x] Composite Component - a component that is made of many other components, but presents the interface of being just one component.
 2. [x] Solid Frame - a frame made of solid characters.
 3. [x] Named Frame - a frame that displays text in addition to the solid characters.
 
 ## Buttons
 Implement clickable buttons
 1. [x] Button - a normal click button
 2. [x] Toggle Button - a button that can be toggled on [x] and off [ ].
 
 ## Text Areas
 Fundamentals for all types of text components:
 1. [X] Viewport - a component that sees a smaller portion of the underlying component
 2. [X] Horizontal Scroll Bar
 3. [X] Vertical Scroll Bar
 4. [X] Scroll Frame - a frame that includes scroll bars
 5. [X] Scroll Pane - a component that includes a scroll frame and a viewport
 
 Specific text manipulation components:
 1. [x] Edit - a single-lined horizontally scrolling text box, with a frame.
 2. [x] Text Area - a multiple-lined vertically scrolling text box, with a frame.
 
 ## Lists
 1. [x] List - A vertical list of elements that can be individually selected.

# Status

Munin is automatically tested with MSVC 2019 and GCC 7.5.  For further information about the working status of Munin to report any bugs, or to make any feature requests, visit the [issues page](https://github.com/KazDragon/munin/issues).  Feel free to discuss using Github Discussions!

# The Basics

The purpose of the Munin library is to provide a windowed component and layout framework for terminal applications, such as those executed on the command line, but also especially those provided by a Telnet server (e.g. a MUD, or chatter).  Integration with either the command line or with a Telnet server is not provided by Munin; that is left to the integrator of the library.  Examples can be found nearby this repository.  For example, the Paradice9 (a chatter with tabletop roleplaying extensions) and Textray (a raycasting 3D room renderer) projects are both built using Server++, Telnet++, Terminal++, and Munin.

# Component

The core abstraction within Munin is the component.  Everything that is drawable in one way or another derives from this abstraction so that they can provide knowledge about where they are (position), how large they are (size), how large they would like to be (preferred size), what to do if a user interacts with it (event) and how to represent themselves on a canvas (draw).

# Container / Layout

Another key abstraction within Munin is the container.  A container, when combined with a layout, declares how to arrange a set of components.  Some layouts, for example the grid layout, simply place components in known positions according to the order in which they are added to the container.  Other layouts, such as the compass and aligned layouts, require the developer to supply hints about where to place each component.  The end effect is that the container automatically adjusts the size and position of its components according to how large the container itself is.

# Window

At the top of the stack is the window.  This is where the underlying application integrates with Munin.  A window is not a component, but is instead constructed with a component that represents the user interface to be displayed.  From here, events can be sent to the window's components.  Ideally, these will be directly from Terminal++, and the default set of components are built with this in mind.  But the interface is flexible enough to allow custom messages to be propagated to custom components.  Additionally, you can register to receive a callback whenever something in the interface changes that requires repainting, and you can of course command to the window to repaint itself.  This returns a string of text that you can send to a terminal to effect the repaint on the client.
