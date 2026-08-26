#pragma once

#include <terminalpp/attribute.hpp>
#include <terminalpp/extent.hpp>
#include <terminalpp/graphics.hpp>
#include <terminalpp/point.hpp>

#include <vector>

namespace paint {

class paint_model
{
public:
    explicit paint_model(terminalpp::extent size)
      : size_{size}, cells_(size.width_ * size.height_, black_cell())
    {
    }

    // Precondition: position is inside the model's extent.
    [[nodiscard]] auto at(terminalpp::point position) const
        -> terminalpp::attribute
    {
        return cells_[index_of(position)];
    }

    void paint(terminalpp::point position)
    {
        cells_[index_of(position)] = selected_brush();
    }

    [[nodiscard]] auto selected_brush() const -> terminalpp::attribute
    {
        auto result = terminalpp::attribute{};
        result.background_colour_ = terminalpp::graphics::colour::white;
        result.intensity_ = terminalpp::graphics::intensity::bold;
        return result;
    }

private:
    [[nodiscard]] static auto black_cell() -> terminalpp::attribute
    {
        auto result = terminalpp::attribute{};
        result.background_colour_ = terminalpp::graphics::colour::black;
        return result;
    }

    [[nodiscard]] auto index_of(terminalpp::point position) const -> int
    {
        return position.y_ * size_.width_ + position.x_;
    }

    terminalpp::extent size_;
    std::vector<terminalpp::attribute> cells_;
};

}  // namespace paint
