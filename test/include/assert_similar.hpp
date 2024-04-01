#pragma once

#include <gsl/gsl-lite.hpp>
#include <gtest/gtest.h>
#include <terminalpp/canvas.hpp>
#include <terminalpp/rectangle.hpp>
#include <terminalpp/string.hpp>

void assert_similar_canvas_block(
    std::initializer_list<terminalpp::string const> expected,
    terminalpp::canvas const &cvs);

void assert_similar_canvas_block(
    std::initializer_list<terminalpp::string const> expected,
    terminalpp::canvas const &cvs,
    terminalpp::rectangle const &bounds);

void assert_similar_canvas_block(
    gsl::span<terminalpp::string const> expected,
    terminalpp::canvas const &cvs);

void assert_similar_canvas_block(
    gsl::span<terminalpp::string const> expected,
    terminalpp::canvas const &cvs,
    terminalpp::rectangle const &bounds);