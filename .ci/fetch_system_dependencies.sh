#!/bin/bash
if [ -z ${EXTERNAL_ROOT+x} ]; then echo EXTERNAL_ROOT not set; exit 1; fi

export EXTERNAL_BUILD_ROOT=$HOME/external_build

mkdir "$EXTERNAL_BUILD_ROOT" || true

# Note: Boost and libfmt are install from apt in build.yml.

# Install nlohmann_json dependency
if [ ! -f "$EXTERNAL_ROOT/include/nlohmann/json.hpp" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/nlohmann/json/archive/v3.12.0.tar.gz;
    tar -xzf v3.12.0.tar.gz;
    cd json-3.12.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DJSON_BuildTests=Off .;
    make -j2 && make install;
fi

# Install Terminal++ dependency
if [ ! -f "$EXTERNAL_ROOT/include/terminalpp-4.0.1/terminalpp/version.hpp" ]; then
    wget https://github.com/KazDragon/terminalpp/archive/v4.0.1.tar.gz;
    tar -xzf v4.0.1.tar.gz;
    cd terminalpp-4.0.1;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DCMAKE_PREFIX_PATH="$EXTERNAL_ROOT" -DTERMINALPP_WITH_TESTS=False -DTERMINALPP_VERSION="4.0.1" .;
    make -j2 && make install;
    cd ..;
fi

# Install Console++ dependency
if [ ! -f "$EXTERNAL_ROOT/include/consolepp-0.2.1/consolepp/version.hpp" ]; then
    wget https://github.com/KazDragon/consolepp/archive/v0.2.1.tar.gz;
    tar -xzf v0.2.1.tar.gz;
    cd consolepp-0.2.1;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DCMAKE_PREFIX_PATH="$EXTERNAL_ROOT" -DCONSOLEPP_WITH_TESTS=False -DCONSOLEPP_VERSION="0.2.1".;
    make -j2 && make install;
    cd ..;
fi

# Install gtest dependency
if [ ! -f "$EXTERNAL_ROOT/include/gtest/gtest.h" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz;
    tar -xzf googletest-1.17.0.tar.gz;
    cd googletest-1.17.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" .;
    make -j2 && make install;
fi

echo Finished installing dependencies.
