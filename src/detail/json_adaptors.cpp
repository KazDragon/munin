#include "munin/detail/json_adaptors.hpp"
#include <terminalpp/point.hpp>
#include <terminalpp/extent.hpp>
#include <terminalpp/string.hpp>

namespace munin { namespace detail {

nlohmann::json to_json(terminalpp::point const &pt)
{
    return {
        { "x", pt.x_ },
        { "y", pt.y_ }
    };
}

nlohmann::json to_json(terminalpp::extent const &ext)
{
    return {
        { "width", ext.width_ },
        { "height", ext.height_ }
    };
}

nlohmann::json to_json(terminalpp::element const &elem)
{
    return terminalpp::to_string(terminalpp::string{elem});
}

}}

