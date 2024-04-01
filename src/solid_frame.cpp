#include "munin/solid_frame.hpp"

#include "munin/compass_layout.hpp"
#include "munin/detail/adaptive_fill.hpp"
#include "munin/view.hpp"

#include <boost/make_unique.hpp>

namespace munin {

// ==========================================================================
// SOLID_FRAME::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct solid_frame::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    explicit impl(solid_frame &self) : self(self)
    {
    }

    // ======================================================================
    // REDRAW_FRAME
    // ======================================================================
    void redraw_frame()
    {
        auto size = self.get_size();

        if (size.width_ > 2 && size.height_ > 2)
        {
            // Here we individually pick out the frame edges and redraw them.
            auto north_beam_region = terminalpp::rectangle{
                {0,           0},
                {size.width_, 1}
            };
            auto south_beam_region = terminalpp::rectangle{
                {0,           size.height_ - 1},
                {size.width_, 1               }
            };
            auto west_beam_region = terminalpp::rectangle{
                {0, 1               },
                {1, size.height_ - 2}
            };
            auto east_beam_region = terminalpp::rectangle{
                {size.width_ - 1, 1               },
                {1,               size.height_ - 2}
            };

            self.on_redraw(
                {north_beam_region,
                 south_beam_region,
                 west_beam_region,
                 east_beam_region});
        }
        else
        {
            // But if only our border is showing, then the redraw region is
            // the complete frame area.
            self.on_redraw({
                {{0, 0}, size}
            });
        }
    }

    solid_frame &self;
    terminalpp::attribute current_attribute;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
solid_frame::solid_frame() : pimpl_(boost::make_unique<impl>(*this))
{
    auto &attr = pimpl_->current_attribute;

    auto north_beam = view(
        make_compass_layout(),
        detail::make_top_left_corner_fill(attr),
        compass_layout::heading::west,
        detail::make_horizontal_beam_fill(attr),
        compass_layout::heading::centre,
        detail::make_top_right_corner_fill(attr),
        compass_layout::heading::east);

    auto south_beam = view(
        make_compass_layout(),
        detail::make_bottom_left_corner_fill(attr),
        compass_layout::heading::west,
        detail::make_horizontal_beam_fill(attr),
        compass_layout::heading::centre,
        detail::make_bottom_right_corner_fill(attr),
        compass_layout::heading::east);

    auto west_beam = detail::make_vertical_beam_fill(attr);
    auto east_beam = detail::make_vertical_beam_fill(attr);

    set_layout(make_compass_layout());

    add_component(north_beam, compass_layout::heading::north);
    add_component(south_beam, compass_layout::heading::south);
    add_component(west_beam, compass_layout::heading::west);
    add_component(east_beam, compass_layout::heading::east);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
solid_frame::~solid_frame() = default;

// ==========================================================================
// NORTH_BORDER_HEIGHT
// ==========================================================================
terminalpp::coordinate_type solid_frame::north_border_height() const
{
    return 1;
}

// ==========================================================================
// SOUTH_BORDER_HEIGHT
// ==========================================================================
terminalpp::coordinate_type solid_frame::south_border_height() const
{
    return 1;
}

// ==========================================================================
// WEST_BORDER_WIDTH
// ==========================================================================
terminalpp::coordinate_type solid_frame::west_border_width() const
{
    return 1;
}

// ==========================================================================
// EAST_BORDER_WIDTH
// ==========================================================================
terminalpp::coordinate_type solid_frame::east_border_width() const
{
    return 1;
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json solid_frame::do_to_json() const
{
    nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "solid_frame" }
    ])"_json;

    return composite_component::do_to_json().patch(patch);
}

// ==========================================================================
// DO_INNER_FOCUS_CHANGED
// ==========================================================================
void solid_frame::do_inner_focus_changed()
{
    pimpl_->current_attribute = get_focus_attribute();
    pimpl_->redraw_frame();
}

// ==========================================================================
// MAKE_SOLID_FRAME
// ==========================================================================
std::shared_ptr<solid_frame> make_solid_frame()
{
    return std::make_shared<solid_frame>();
}

// ==========================================================================
// MAKE_SOLID_FRAME
// ==========================================================================
std::shared_ptr<solid_frame> make_solid_frame(
    std::shared_ptr<component> const &associated_component)
{
    auto frame = make_solid_frame();
    frame->highlight_on_focus(associated_component);
    return frame;
}

}  // namespace munin
