#pragma once
#include <terminalpp/point.hpp>
#include <terminalpp/extent.hpp>
#include <json.hpp>

namespace munin { namespace detail {

nlohmann::json to_json(terminalpp::point const &pt)
{
    return {
        { "x", pt.x },
        { "y", pt.y }
    };
}

nlohmann::json to_json(terminalpp::extent const &ext)
{
    return {
        { "width", ext.width },
        { "height", ext.height }
    };
}

}}
