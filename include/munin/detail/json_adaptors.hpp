#pragma once
#include <nlohmann/json.hpp>

namespace terminalpp {
    struct point;
    struct extent;
    struct glyph;
    struct attribute;
    struct element;
}

namespace munin { namespace detail {

nlohmann::json to_json(terminalpp::point const &pt);
nlohmann::json to_json(terminalpp::extent const &ext);
nlohmann::json to_json(terminalpp::element const &elem);

}}
