#!/bin/bash
if [ -z ${EXTERNAL_ROOT+x} ]; then echo EXTERNAL_ROOT not set; exit 1; fi

export EXTERNAL_BUILD_ROOT=$HOME/external_build

mkdir "$EXTERNAL_BUILD_ROOT" || true

# Install nlohmann_json dependency
if [ ! -f "$EXTERNAL_ROOT/include/nlohmann/json.hpp" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/nlohmann/json/archive/v3.3.0.tar.gz;
    tar -xzf v3.3.0.tar.gz;
    cd json-3.3.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DJSON_BuildTests=Off .;
    make -j2 && make install;
fi

# Install fmt dependency
if [ ! -f "$EXTERNAL_ROOT/include/fmt/format.hpp" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/fmtlib/fmt/archive/7.1.2.tar.gz;
    tar -xzf 7.1.2.tar.gz;
    cd fmt-7.1.2;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DFMT_TEST=Off .;
    make -j2 && make install;
fi

# Install gsl-lite dependency
if [ ! -f "$EXTERNAL_ROOT/include/gsl/gsl-lite.hpp" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/gsl-lite/gsl-lite/archive/v0.34.0.tar.gz;
    tar -xzf v0.34.0.tar.gz;
    cd gsl-lite-0.34.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DGSL_LITE_OPT_BUILD_TESTS=Off .;
    make -j2 && make install;
fi

# Install Terminal++ dependency
if [ ! -f "$EXTERNAL_ROOT/include/terminalpp/version/hpp" ]; then
    wget https://github.com/KazDragon/terminalpp/archive/v2.0.2.tar.gz;
    tar -xzf v2.0.2.tar.gz;
    cd terminalpp-2.0.2;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" -DTERMINALPP_WITH_TESTS=False .;
    make -j2 && make install;
    cd ..;
fi

# Install gtest dependency
if [ ! -f "$EXTERNAL_ROOT/include/gtest/gtest.h" ]; then
    cd "$EXTERNAL_BUILD_ROOT";
    wget https://github.com/google/googletest/archive/release-1.10.0.tar.gz;
    tar -xzf release-1.10.0.tar.gz;
    cd googletest-release-1.10.0;
    cmake -DCMAKE_INSTALL_PREFIX="$EXTERNAL_ROOT" .;
    make -j2 && make install;
fi
