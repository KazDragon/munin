# Munin

[![Documentation](https://codedocs.xyz/KazDragon/munin.svg)](https://codedocs.xyz/KazDragon/munin/)
[![License](https://img.shields.io/github/license/KazDragon/munin.svg)](https://en.wikipedia.org/wiki/MIT_License)
[![Build Status](https://travis-ci.org/KazDragon/munin.svg?branch=master)](https://travis-ci.org/KazDragon/munin)
[![Coverage Status](https://coveralls.io/repos/github/KazDragon/munin/badge.svg?branch=master)](https://coveralls.io/github/KazDragon/munin?branch=master)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/cbf22a847dc040b1a9dee8be3eda00d3)](https://www.codacy.com/app/KazDragon/munin?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=KazDragon/munin&amp;utm_campaign=Badge_Grade)

[![Github Issues](https://img.shields.io/github/issues/KazDragon/munin.svg)](https://github.com/KazDragon/munin/issues)
[![Stories in Ready](https://badge.waffle.io/KazDragon/munin.png?label=ready&title=Stories%20In%20Ready)](https://waffle.io/KazDragon/munin)
[![Stories in Progress](https://badge.waffle.io/KazDragon/munin.png?label=in%20progress&title=Stories%20In%20Progress)](https://waffle.io/KazDragon/munin)

[![Gitter](https://badges.gitter.im/KazDragon/munin.svg)](https://gitter.im/KazDragon/munin?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)
[![This is a forkable respository](https://img.shields.io/badge/forkable-yes-brightgreen.svg)](https://basicallydan.github.io/forkability/?u=KazDragon&r=munin)

Munin is an enhancement to the Terminal++ library that provides a set of component-based UI widgets.

# Requirements

Munin requires a C++14 compiler, Terminal++ and the Boost Libraries.  It also uses Google Test and Google Mock for its testing suite, which are compiled optionally.

# Features / Roadmap / Progress

## v0.1 - Fundamentals
Implement the fundamentals of the library, providing the basis for further development of the library.
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

 ## v0.2 - Frames
 Implement the ability to frame components
 1. [x] Composite Component - a component that is made of many other components, but presents the interface of being just one component.
 2. [x] Solid Frame - a frame made of solid characters.
 3. [x] Named Frame - a frame that displays text in addition to the solid characters.
 
 ## v0.3 - Buttons
 Implement clickable buttons
 1. [ ] Button - a normal click button
 2. [ ] Toggle Button - a button that can be toggled on [x] and off [ ].
 
 ## v0.4 - Text Areas
 1. [ ] Fundamentals for all types of text components
  * [ ] Viewport - a component that sees a smaller portion of the underlying component
  * [ ] Horizontal Scroll Bar
  * [ ] Vertical Scroll Bar
  * [ ] Scroll Frame - a frame that includes scroll bars
  * [ ] Scroll Pane - a component that includes a scroll frame and a viewport
 2. [ ] Edit - a single-lined horizontally scrolling text box, with a frame.
 3. [ ] Text Area - a multiple-lined vertically scrolling text box, with a frame.
 
 ## v0.5 - Lists
 TBD


# Status

Munin is automatically tested with Clang and G++ 5.2.

For further information about the working status of Munin to report any bugs, or to make any feature requests, visit the [Waffle board](https://waffle.io/KazDragon/munin)

# The Basics

The purpose of the Munin library is to provide a windowed component and layout framework for terminal applications, such as those executed on the command line, but also especially those provided by a Telnet server (e.g. a MUD, or chatter).  Integration with either the command line or with a Telnet server is not provided by Munin; that is left to the integrator of the library.  Examples can be found neaby this repository.  For example, the Paradice9 (a chatter with tabletop roleplaying extensions) and Munin-Acceptance (a raycasting 3D room renderer) projects are both built on Telnet++, Terminal++, and Munin.

# Component

The core abstraction within Munin is the component.  Everything that is drawable in one way or another derives from this abstraction so that they can provide knowledge about where they are (position), how large they are (size), how large they would like to be (preferred size), what to do if a user interacts with it (event) and how to represent themselves on a canvas (draw).

# Container / Layout

Another key abstraction within Munin is the container.  A container, when combined with a layout, declares how to arrange a set of components.  Some layouts, for example the grid layout, simply place components in known positions according to the order in which they are added to the container.  Other layouts, such as the compass and aligned layouts, require the developer to supply hints about where to place each component.  The end effect is that the container automatically adjusts the size and position of its components according to how large the container itself is.

# Window

At the top of the stack is the window.  This is where the underlying application integrates with Munin.  A window is not a component, but is instead constructed with a component that represents the user interface to be displayed.  From here, events can be sent to the window's components.  Ideally, these will be directly from Terminal++, and the default set of components are built with this in mind.  But the interface is flexible enough to allow custom messages to be propagated to custom components.  Additionally, you can register to receive a callback whenever something in the interface changes that requires repainting, and you can of course command to the window to repaint itself.  This returns a string of text that you can send to a terminal to effect the repaint on the client.

