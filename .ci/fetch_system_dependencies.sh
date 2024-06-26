#!/bin/bash
if [ -z ${EXTERNAL_ROOT+x} ]; then echo EXTERNAL_ROOT not set; exit 1; fi

export EXTERNAL_BUILD_ROOT=$HOME/external_build

mkdir "$EXTERNAL_BUILD_ROOT" || true

# Note: Boost and libfmt are install from apt in build.yml.

# Install nlohmann_json dependency
if [ ! -f "$EXTERNAL_ROOT/include/nlohmann/json.hpp" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/nlohmann/json/archive/v3.3.0.tar.gz;
    tar -xzf v3.3.0.tar.gz;
    cd json-3.3.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DJSON_BuildTests=Off .;
    make -j2 && make install;
fi

# Install gsl-lite dependency
if [ ! -f "$EXTERNAL_ROOT/include/gsl/gsl-lite.hpp" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/gsl-lite/gsl-lite/archive/v0.38.0.tar.gz;
    tar -xzf v0.38.0.tar.gz;
    cd gsl-lite-0.38.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DGSL_LITE_OPT_BUILD_TESTS=Off .;
    make -j2 && make install;
fi

# Install Terminal++ dependency
if [ ! -f "$EXTERNAL_ROOT/include/terminalpp-3.1.0/terminalpp/version.hpp" ]; then
    wget https://github.com/KazDragon/terminalpp/archive/v3.1.0.tar.gz;
    tar -xzf v3.1.0.tar.gz;
    cd terminalpp-3.1.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DCMAKE_PREFIX_PATH="$EXTERNAL_ROOT" -DTERMINALPP_WITH_TESTS=False -DTERMINALPP_VERSION="3.1.0" .;
    make -j2 && make install;
    cd ..;
fi

# Install Console++ dependency
if [ ! -f "$EXTERNAL_ROOT/include/consolepp-0.1.1/consolepp/version.hpp" ]; then
    wget https://github.com/KazDragon/consolepp/archive/v0.1.1.tar.gz;
    tar -xzf v0.1.1.tar.gz;
    cd consolepp-0.1.1;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DCMAKE_PREFIX_PATH="$EXTERNAL_ROOT" -DCONSOLEPP_WITH_TESTS=False .;
    make -j2 && make install;
    cd ..;
fi

# Install gtest dependency
if [ ! -f "$EXTERNAL_ROOT/include/gtest/gtest.h" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/google/googletest/archive/release-1.11.0.tar.gz;
    tar -xzf release-1.11.0.tar.gz;
    cd googletest-release-1.11.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" .;
    make -j2 && make install;
fi

echo Finished installing dependencies.
