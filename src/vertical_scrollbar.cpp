#include "munin/vertical_scrollbar.hpp"
#include "munin/detail/border_glyphs.hpp"
#include "munin/render_surface.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/mouse.hpp>
#include <boost/make_unique.hpp>
#include <memory>

namespace munin {

// ==========================================================================
// VERTICAL_SCROLLBAR IMPLEMENTATION STRUCTURE
// ==========================================================================
struct vertical_scrollbar::impl
{
  // ======================================================================
  // CONSTRUCTOR
  // ======================================================================
  explicit impl(vertical_scrollbar &self) : self(self)
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
      self.on_redraw({{{}, self.get_size()}});
    }
  }

  // ======================================================================
  // CALCULATE_SLIDER_POSITION
  // ======================================================================
  void calculate_slider_position(
      terminalpp::coordinate_type const scrollbar_height)
  {
    if (viewport_basis_height == 0)
    {
      slider_position = std::nullopt;
    }
    else
    {
      // The slider is in the topmost position only if the viewport
      // y position is precisely 0.
      auto const &slider_is_in_topmost_position = [=]
      { return viewport_y_position == 0; };

      // The slider is in the bottommost position only if the viewport
      // basis is as far down as it can be.
      auto const &slider_is_in_bottommost_position = [=]
      { return viewport_y_position == viewport_basis_height; };

      auto const &interpolate_slider_position = [=]
      {
        // There are scrollbar_height - 2 possible positions.
        // The leftmost but one starts at 1, and increments
        // by (scrollbar_height - 2) / (viewport_basis_height - 2)
        // per viewport_y_position.
        auto const slider_positions = scrollbar_height - 2;
        auto const viewport_basis_positions = viewport_basis_height - 2;

        // Starting from co-ordinate 1, increment by
        // slider_positions / viewport_basis_height per viewport y
        // position
        return 1
               + (((viewport_y_position - 1) * slider_positions)
                  / viewport_basis_positions);
      };

      // Otherwise, it is an interpolated position between those points.
      slider_position = slider_is_in_topmost_position() ? 0
                        : slider_is_in_bottommost_position()
                            ? (scrollbar_height - 1)
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
        if (mouse_event.position_.y_ < *slider_position)
        {
          self.on_scroll_up();
        }
        else if (mouse_event.position_.y_ > *slider_position)
        {
          self.on_scroll_down();
        }
      }
    }
  }

  vertical_scrollbar &self;

  std::weak_ptr<component> associated_component;
  terminalpp::attribute lowlight_attribute;
  terminalpp::attribute highlight_attribute;
  bool associated_component_has_focus = false;

  terminalpp::coordinate_type viewport_y_position = 0;
  terminalpp::coordinate_type viewport_basis_height = 0;
  std::optional<terminalpp::coordinate_type> slider_position;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
vertical_scrollbar::vertical_scrollbar()
  : pimpl_(boost::make_unique<impl>(*this))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
vertical_scrollbar::~vertical_scrollbar() = default;

// ==========================================================================
// SET_SLIDER_POSITION
// ==========================================================================
void vertical_scrollbar::set_slider_position(
    terminalpp::coordinate_type y_position,  // NOLINT
    terminalpp::coordinate_type height)
{
  pimpl_->viewport_y_position = y_position;
  pimpl_->viewport_basis_height = height;
  pimpl_->calculate_slider_position(get_size().height_);

  on_redraw({{{}, get_size()}});
}

// ==========================================================================
// HIGHLIGHT_ON_FOCUS
// ==========================================================================
void vertical_scrollbar::highlight_on_focus(
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
void vertical_scrollbar::set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
  pimpl_->lowlight_attribute = lowlight_attribute;
  pimpl_->redraw_according_to_associated_focus();
}

// ==========================================================================
// SET_HIGHLIGHT_ATTRIBUTE
// ==========================================================================
void vertical_scrollbar::set_highlight_attribute(
    terminalpp::attribute const &highlight_attribute)
{
  pimpl_->highlight_attribute = highlight_attribute;
  pimpl_->redraw_according_to_associated_focus();
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void vertical_scrollbar::do_set_size(terminalpp::extent const &size)
{
  pimpl_->calculate_slider_position(size.height_);
  basic_component::do_set_size(size);
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent vertical_scrollbar::do_get_preferred_size() const
{
  return {1, get_size().height_};
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void vertical_scrollbar::do_draw(
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
        bool const at_slider_position = row == pimpl_->slider_position;

        elem = {
            supports_unicode
                ? at_slider_position
                      ? munin::detail::border::unicode::cross
                      : munin::detail::border::unicode::vertical_beam
            : at_slider_position ? munin::detail::border::ansi::cross
                                 : munin::detail::border::ansi::vertical_beam,
            attribute};
      });
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void vertical_scrollbar::do_event(boost::any const &event)
{
  if (auto const *mouse_event =
          boost::any_cast<terminalpp::mouse::event>(&event);
      mouse_event)
  {
    pimpl_->handle_mouse_event(*mouse_event);
  }
}

// ==========================================================================
// MAKE_VERTICAL_SCROLLBAR
// ==========================================================================
std::shared_ptr<vertical_scrollbar> make_vertical_scrollbar()
{
  return std::make_shared<vertical_scrollbar>();
}

}  // namespace munin