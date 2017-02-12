#include "munin/grid_layout.hpp"
#include "munin/container.hpp"

namespace munin {

// ==========================================================================
// GRID_LAYOUT::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct grid_layout::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl()
        : rows_(0)
        , columns_(0)
    {
    }

    odin::u32 rows_;
    odin::u32 columns_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
grid_layout::grid_layout(odin::u32 rows, odin::u32 columns)
    : pimpl_(std::make_shared<impl>())
{
    pimpl_->rows_    = rows;
    pimpl_->columns_ = columns;
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
grid_layout::~grid_layout()
{
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent grid_layout::do_get_preferred_size(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints) const
{
    // The preferred size of the whole component is the maximum preferred
    // width and the maximum preferred height of all components,
    // multiplied appropriately by the rows and columns
    terminalpp::extent maximum_preferred_size(0, 0);

    for (auto comp : components)
    {
        auto preferred_size = comp->get_preferred_size();

        maximum_preferred_size.width = (std::max)(
            maximum_preferred_size.width
          , preferred_size.width);

        maximum_preferred_size.height = (std::max)(
            maximum_preferred_size.height
          , preferred_size.height);
    }

    maximum_preferred_size.width  *= pimpl_->columns_;
    maximum_preferred_size.height *= pimpl_->rows_;

    return maximum_preferred_size;
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void grid_layout::do_layout(
    std::vector<std::shared_ptr<component>> const &components,
    std::vector<boost::any>                 const &hints,
    terminalpp::extent                             size)
{
    for (odin::u32 index = 0; index < components.size(); ++index)
    {
        auto comp = components[index];

        // Work out the row/column of the current component.
        odin::u32 row    = index / pimpl_->columns_;
        odin::u32 column = index % pimpl_->columns_;

        // Naive: will have missing pixels and off-by-one errors
        comp->set_position(
            terminalpp::point(
                (size.width / pimpl_->columns_) * column
              , (size.height / pimpl_->rows_) * row));

        comp->set_size(
            terminalpp::extent(
                size.width  / pimpl_->columns_
              , size.height / pimpl_->rows_));
    }
}

// ==========================================================================
// MAKE_GRID_LAYOUT
// ==========================================================================
std::unique_ptr<layout> make_grid_layout(odin::u32 rows, odin::u32 columns)
{
    return std::unique_ptr<layout>(new grid_layout(rows, columns));
}

}

