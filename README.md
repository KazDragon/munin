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
3. [ ] A sampling of common layouts 
 * [x] Null Layout - a layout that doesn't.
 * [x] Vertical Strip Layout - a layout that arranges components in vertical strips
 * [x] Horizontal Strip Layout - a layout that arranges components in horizontal strips
 * [x] Grid Layout - a layout that arranges components in equally-sized cells.
 * [x] Aligned Layout - a layout that arranges components with horizontal and vertical alignment (e.g. top-left, right-center)
 * [ ] Compass Layout - a layout that arranges components according to compass points (e.g. North, South, Centre)
 4. [ ] A sampling of common components
 * [x] Filled Box - a component that simply paints a single repeated character
 * [x] Brush - a component that paints a repeated character pattern.
 * [x] Image - a component that paints a static "image" once.
 * [ ] Window - a top-level object that contains and draws components onto a canvas.
 
 ## v0.2 - Frames
 Implement the ability to frame components
 1. [ ] Composite Component - a component that is made of many other components, but presents the interface of being just one component.
 2. [ ] Solid Frame - a frame made of solid characters.
 3. [ ] Named Frame - a frame that displays text in addition to the solid characters.
 
 ## v0.3 - Buttons
 Implement clickable buttons
 1. [ ] Button - a normal click button
 2. [ ] Toggle Button - a button that can be toggled on [x] and off [ ].
 
 ## v0.4 - Text Areas
 TBD
 
 ## v0.5 - Lists
 TBD


# Status

Munin is automatically tested with Clang and G++ 5.2.

For further information about the working status of Munin to report any bugs, or to make any feature requests, visit the [Waffle board](https://waffle.io/KazDragon/munin)

