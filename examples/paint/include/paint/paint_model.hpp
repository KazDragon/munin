#pragma once

#include <terminalpp/attribute.hpp>
#include <terminalpp/extent.hpp>
#include <terminalpp/graphics.hpp>
#include <terminalpp/point.hpp>

namespace paint {

class paint_model
{
public:
    explicit paint_model(terminalpp::extent size) : size_{size}
    {
    }

    // Precondition: position is inside the model's extent.
    [[nodiscard]] auto at(terminalpp::point position) const
        -> terminalpp::attribute
    {
        auto result = terminalpp::attribute{};
        result.background_colour_ = terminalpp::graphics::colour::black;
        return result;
    }

    [[nodiscard]] auto selected_brush() const -> terminalpp::attribute
    {
        auto result = terminalpp::attribute{};
        result.background_colour_ = terminalpp::graphics::colour::white;
        result.intensity_ = terminalpp::graphics::intensity::bold;
        return result;
    }

private:
    terminalpp::extent size_;
};

}  // namespace paint
