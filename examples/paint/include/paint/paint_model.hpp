#pragma once

#include <terminalpp/attribute.hpp>
#include <terminalpp/extent.hpp>
#include <terminalpp/graphics.hpp>
#include <terminalpp/point.hpp>

#include <algorithm>
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
        if (is_in_bounds(position))
        {
            cells_[index_of(position)] = selected_brush();
        }
    }

    void resize(terminalpp::extent size)
    {
        auto new_cells = std::vector<terminalpp::attribute>(
            size.width_ * size.height_, black_cell());
        auto const preserved_width = std::min(size_.width_, size.width_);
        auto const preserved_height = std::min(size_.height_, size.height_);

        for (auto y = 0; y < preserved_height; ++y)
        {
            for (auto x = 0; x < preserved_width; ++x)
            {
                auto const old_position = terminalpp::point{x, y};
                auto const new_index = y * size.width_ + x;
                new_cells[new_index] = cells_[index_of(old_position)];
            }
        }

        size_ = size;
        cells_ = std::move(new_cells);
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

    [[nodiscard]] auto is_in_bounds(terminalpp::point position) const -> bool
    {
        return position.x_ >= 0 && position.x_ < size_.width_
            && position.y_ >= 0 && position.y_ < size_.height_;
    }

    terminalpp::extent size_;
    std::vector<terminalpp::attribute> cells_;
};

}  // namespace paint
