#include "munin/horizontal_scrollbar.hpp"
#include "munin/detail/border_glyphs.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>
#include <boost/make_unique.hpp>
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
  // REDRAW_ACCORDING_TO_ASSOCIATED_FOCUS
  // ======================================================================
  void redraw_according_to_associated_focus()
  {
    if (auto comp = associated_component.lock(); comp)
    {
      associated_component_has_focus = comp->has_focus();
      self_.on_redraw({{{}, self_.get_size()}});
    }
  }

  // ======================================================================
  // CALCULATE_SLIDER_POSITION
  // ======================================================================
  void calculate_slider_position(
      terminalpp::coordinate_type const scrollbar_width)
  {
    if (viewport_basis_width == 0)
    {
      slider_position = std::nullopt;
    }
    else
    {
      // The slider is in the leftmost position only if the viewport
      // x position is precisely 0.
      auto const &slider_is_in_leftmost_position = [=]
      { return viewport_x_position == 0; };

      // The slider is in the rightmost position only if the viewport
      // basis is as far right as it can be.
      auto const &slider_is_in_rightmost_position = [=]
      { return viewport_x_position == viewport_basis_width; };

      auto const &interpolate_slider_position = [=]
      {
        // There are scrollbar_width - 2 possible positions.
        // The leftmost but one starts at 1, and increments
        // by (scrollbar_width - 2) / (viewport_basis_width - 2)
        // per viewport_x_position.
        auto const slider_positions = scrollbar_width - 2;
        auto const viewport_basis_positions = viewport_basis_width - 2;

        // Starting from co-ordinate 1, increment by
        // slider_positions / viewport_basis_width per viewport x
        // position
        return 1
               + (((viewport_x_position - 1) * slider_positions)
                  / viewport_basis_positions);
      };

      // Otherwise, it is an interpolated position between those points.
      slider_position = slider_is_in_leftmost_position() ? 0
                        : slider_is_in_rightmost_position()
                            ? (scrollbar_width - 1)
                            : interpolate_slider_position();
    }
  }

  // ======================================================================
  // HANDLE_MOUSE_EVENT
  // ======================================================================
  void handle_mouse_event(terminalpp::mouse::event const &mouse_event)
  {
    if (mouse_event.action_ == terminalpp::mouse::event_type::left_button_down)
    {
      if (slider_position.has_value())
      {
        if (mouse_event.position_.x_ < *slider_position)
        {
          self_.on_scroll_left();
        }
        else if (mouse_event.position_.x_ > *slider_position)
        {
          self_.on_scroll_right();
        }
      }
    }
  }

  horizontal_scrollbar &self_;  // NOLINT

  std::weak_ptr<component> associated_component;
  terminalpp::attribute lowlight_attribute;
  terminalpp::attribute highlight_attribute;
  bool associated_component_has_focus = false;

  terminalpp::coordinate_type viewport_x_position = 0;
  terminalpp::coordinate_type viewport_basis_width = 0;
  std::optional<terminalpp::coordinate_type> slider_position;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
horizontal_scrollbar::horizontal_scrollbar()
  : pimpl_(boost::make_unique<impl>(*this))
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
  pimpl_->viewport_x_position = x_position;
  pimpl_->viewport_basis_width = width;
  pimpl_->calculate_slider_position(get_size().width_);

  on_redraw({{{}, get_size()}});
}

// ==========================================================================
// HIGHLIGHT_ON_FOCUS
// ==========================================================================
void horizontal_scrollbar::highlight_on_focus(
    std::shared_ptr<component> const &associated_component)
{
  pimpl_->associated_component = associated_component;

  associated_component->on_focus_set.connect(
      [this] { pimpl_->redraw_according_to_associated_focus(); });
  associated_component->on_focus_lost.connect(
      [this] { pimpl_->redraw_according_to_associated_focus(); });

  pimpl_->redraw_according_to_associated_focus();
}

// ==========================================================================
// SET_LOWLIGHT_ATTRIBUTE
// ==========================================================================
void horizontal_scrollbar::set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
  pimpl_->lowlight_attribute = lowlight_attribute;
  pimpl_->redraw_according_to_associated_focus();
}

// ==========================================================================
// SET_HIGHLIGHT_ATTRIBUTE
// ==========================================================================
void horizontal_scrollbar::set_highlight_attribute(
    terminalpp::attribute const &highlight_attribute)
{
  pimpl_->highlight_attribute = highlight_attribute;
  pimpl_->redraw_according_to_associated_focus();
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
  auto const &attribute = pimpl_->associated_component_has_focus
                              ? pimpl_->highlight_attribute
                              : pimpl_->lowlight_attribute;

  bool const supports_unicode = surface.supports_unicode();

  terminalpp::for_each_in_region(
      surface,
      region,
      [this, &attribute, supports_unicode](
          terminalpp::element &elem,
          terminalpp::coordinate_type column,  // NOLINT
          terminalpp::coordinate_type row)
      {
        bool const at_slider_position = column == pimpl_->slider_position;

        elem = {
            supports_unicode
                ? at_slider_position
                      ? munin::detail::border::unicode::cross
                      : munin::detail::border::unicode::horizontal_beam
            : at_slider_position ? munin::detail::border::ansi::cross
                                 : munin::detail::border::ansi::horizontal_beam,
            attribute};
      });
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void horizontal_scrollbar::do_event(boost::any const &event)
{
  if (auto const *mouse_event =
          boost::any_cast<terminalpp::mouse::event>(&event);
      mouse_event)
  {
    pimpl_->handle_mouse_event(*mouse_event);
  }
}

// ==========================================================================
// MAKE_HORIZONTAL_SCROLLBAR
// ==========================================================================
std::shared_ptr<horizontal_scrollbar> make_horizontal_scrollbar()
{
  return std::make_shared<horizontal_scrollbar>();
}

}  // namespace munin