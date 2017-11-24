#pragma once
#include <json.hpp>

namespace terminalpp {
    class point;
    class extent;
    class glyph;
    class attribute;
    class element;
}

namespace munin { namespace detail {

nlohmann::json to_json(terminalpp::point const &pt);
nlohmann::json to_json(terminalpp::extent const &ext);
nlohmann::json to_json(terminalpp::element const &elem);

}}
