#include "munin/detail/json_adaptors.hpp"
#include <terminalpp/point.hpp>
#include <terminalpp/extent.hpp>
#include <terminalpp/string.hpp>

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

nlohmann::json to_json(terminalpp::element const &elem)
{
    return terminalpp::to_string(terminalpp::string{elem});
}

}}

