#include "munin/horizontal_scrollbar.hpp"

#include "munin/detail/border_glyphs.hpp"
#include "munin/render_surface.hpp"

#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>

#include <memory>

namespace munin {

// ==========================================================================
// HORIZONTAL_SCROLLBAR IMPLEMENTATION STRUCTURE
// ==========================================================================
struct horizontal_scrollbar::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    explicit impl(horizontal_scrollbar &self) : self_(self)
    {
    }

    // ======================================================================
    // SET_SLIDER_POSITION
    // ======================================================================
    void set_slider_position(
        terminalpp::coordinate_type x_position,  // NOLINT
        terminalpp::coordinate_type width)
    {
        // scroll_pane forwards the viewport anchor origin and its maximum
        // horizontal extent here, not a count of addressable positions.
        viewport_basis_x_position_ = x_position;
        viewport_total_width_ = width;
        calculate_slider_position(self_.get_size().width_);

        self_.on_redraw({
            {{}, self_.get_size()}
        });
    }

    // ======================================================================
    // HIGHLIGHT_ON_FOCUS
    // ======================================================================
    void highlight_on_focus(
        std::shared_ptr<component> const &associated_component)
    {
        associated_component_ = associated_component;

        associated_component->on_focus_set.connect(
            [this] { redraw_according_to_associated_focus(); });
        associated_component->on_focus_lost.connect(
            [this] { redraw_according_to_associated_focus(); });

        redraw_according_to_associated_focus();
    }

    // ======================================================================
    // SET_LOWLIGHT_ATTRIBUTE
    // ======================================================================
    void set_lowlight_attribute(terminalpp::attribute const &lowlight_attribute)
    {
        lowlight_attribute_ = lowlight_attribute;
        redraw_according_to_associated_focus();
    }

    // ======================================================================
    // SET_HIGHLIGHT_ATTRIBUTE
    // ======================================================================
    void set_highlight_attribute(
        terminalpp::attribute const &highlight_attribute)
    {
        highlight_attribute_ = highlight_attribute;
        redraw_according_to_associated_focus();
    }

    // ======================================================================
    // CALCULATE_SLIDER_POSITION
    // ======================================================================
    void calculate_slider_position(
        terminalpp::coordinate_type const scrollbar_width)
    {
        if (viewport_total_width_ == 0)
        {
            slider_position_ = std::nullopt;
        }
        else
        {
            // The viewport contract publishes the maximum anchor origin.
            // That means the terminal endpoints are 0 and viewport_total_width_.
            if (viewport_basis_x_position_ == 0)
            {
                slider_position_ = 0;
            }
            else if (viewport_basis_x_position_ == viewport_total_width_)
            {
                slider_position_ = scrollbar_width - 1;
            }
            else
            {
                auto const slider_positions = scrollbar_width - 2;
                auto const viewport_basis_positions = viewport_total_width_ - 1;

                slider_position_ =
                    1
                  + (((viewport_basis_x_position_ - 1) * slider_positions)
                     / viewport_basis_positions);
            }
        }
    }

    // ======================================================================
    // DRAW
    // ======================================================================
    void draw(
        render_surface &surface, terminalpp::rectangle const &region) const
    {
        auto const &attribute = associated_component_has_focus_
                                  ? highlight_attribute_
                                  : lowlight_attribute_;

        bool const supports_unicode = surface.supports_unicode();

        terminalpp::for_each_in_region(
            surface,
            region,
            [this, &attribute, supports_unicode](
                terminalpp::element &elem,
                terminalpp::coordinate_type column,  // NOLINT
                terminalpp::coordinate_type row) {
                bool const at_slider_position = column == slider_position_;

                elem = {
                    supports_unicode
                        ? at_slider_position
                            ? munin::detail::border::unicode::cross
                            : munin::detail::border::unicode::horizontal_beam
                    : at_slider_position
                        ? munin::detail::border::ansi::cross
                        : munin::detail::border::ansi::horizontal_beam,
                    attribute};
            });
    }

    // ======================================================================
    // HANDLE_EVENT
    // ======================================================================
    void handle_event(std::any const &event)
    {
        if (auto const *mouse_event =
                std::any_cast<terminalpp::mouse::event>(&event);
            mouse_event)
        {
            handle_mouse_event(*mouse_event);
        }
    }

private:
    // ======================================================================
    // REDRAW_ACCORDING_TO_ASSOCIATED_FOCUS
    // ======================================================================
    void redraw_according_to_associated_focus()
    {
        if (auto comp = associated_component_.lock(); comp)
        {
            associated_component_has_focus_ = comp->has_focus();
            self_.on_redraw({
                {{}, self_.get_size()}
            });
        }
    }

    // ======================================================================
    // HANDLE_MOUSE_EVENT
    // ======================================================================
    void handle_mouse_event(terminalpp::mouse::event const &mouse_event)
    {
        if (mouse_event.action_
            == terminalpp::mouse::event_type::left_button_down)
        {
            if (slider_position_.has_value())
            {
                if (mouse_event.position_.x_ < *slider_position_)
                {
                    self_.on_scroll_left();
                }
                else if (mouse_event.position_.x_ > *slider_position_)
                {
                    self_.on_scroll_right();
                }
            }
        }
    }

    horizontal_scrollbar &self_;  // NOLINT

    std::weak_ptr<component> associated_component_;
    terminalpp::attribute lowlight_attribute_;
    terminalpp::attribute highlight_attribute_;
    bool associated_component_has_focus_ = false;

    terminalpp::coordinate_type viewport_basis_x_position_ = 0;
    terminalpp::coordinate_type viewport_total_width_ = 0;
    std::optional<terminalpp::coordinate_type> slider_position_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
horizontal_scrollbar::horizontal_scrollbar()
  : pimpl_(std::make_unique<impl>(*this))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
horizontal_scrollbar::~horizontal_scrollbar() = default;

// ==========================================================================
// SET_SLIDER_POSITION
// ==========================================================================
void horizontal_scrollbar::set_slider_position(
    terminalpp::coordinate_type x_position,  // NOLINT
    terminalpp::coordinate_type width)
{
    pimpl_->set_slider_position(x_position, width);
}

// ==========================================================================
// HIGHLIGHT_ON_FOCUS
// ==========================================================================
void horizontal_scrollbar::highlight_on_focus(
    std::shared_ptr<component> const &associated_component)
{
    pimpl_->highlight_on_focus(associated_component);
}

// ==========================================================================
// SET_LOWLIGHT_ATTRIBUTE
// ==========================================================================
void horizontal_scrollbar::set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
    pimpl_->set_lowlight_attribute(lowlight_attribute);
}

// ==========================================================================
// SET_HIGHLIGHT_ATTRIBUTE
// ==========================================================================
void horizontal_scrollbar::set_highlight_attribute(
    terminalpp::attribute const &highlight_attribute)
{
    pimpl_->set_highlight_attribute(highlight_attribute);
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void horizontal_scrollbar::do_set_size(terminalpp::extent const &size)
{
    pimpl_->calculate_slider_position(size.width_);
    basic_component::do_set_size(size);
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent horizontal_scrollbar::do_get_preferred_size() const
{
    return {get_size().width_, 1};
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void horizontal_scrollbar::do_draw(
    render_surface &surface, terminalpp::rectangle const &region) const
{
    pimpl_->draw(surface, region);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void horizontal_scrollbar::do_event(std::any const &event)
{
    pimpl_->handle_event(event);
}

// ==========================================================================
// MAKE_HORIZONTAL_SCROLLBAR
// ==========================================================================
std::shared_ptr<horizontal_scrollbar> make_horizontal_scrollbar()
{
    return std::make_shared<horizontal_scrollbar>();
}

}  // namespace munin
