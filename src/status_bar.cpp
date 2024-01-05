#include "munin/status_bar.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>
#include <terminalpp/element.hpp>
#include <boost/make_unique.hpp>
#include <munin/animator.hpp>
#include <munin/render_surface.hpp>

namespace munin {

namespace {

using namespace std::literals;

constexpr auto marquee_delay_time = 5s;
constexpr auto marquee_frame_time = 15ms;
constexpr auto characters_per_marquee_frame = 2;

}  // namespace

// ==========================================================================
// STATUS_BAR::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct status_bar::impl
{
  explicit impl(animator &anim) : animator_(anim)
  {
  }

  animator &animator_;          // NOLINT
  terminalpp::string message_;  // NOLINT
  std::optional<std::chrono::steady_clock::time_point>
      marquee_start_time_;  // NOLINT
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
status_bar::status_bar(animator &anim) : pimpl_(boost::make_unique<impl>(anim))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
status_bar::~status_bar() = default;

// ==========================================================================
// SET_MESSAGE
// ==========================================================================
void status_bar::set_message(terminalpp::string const &message)
{
  pimpl_->message_ = message;

  // Reset the marquee start time as an indication that it should start on
  // on the next draw cycle.
  pimpl_->marquee_start_time_ = std::nullopt;

  on_preferred_size_changed();
  on_redraw({{{}, get_size()}});
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent status_bar::do_get_preferred_size() const
{
  return pimpl_->message_.empty() ? terminalpp::extent{1, 1}
                                  : terminalpp::extent{
                                      static_cast<terminalpp::coordinate_type>(
                                          pimpl_->message_.size()),
                                      1};
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void status_bar::do_draw(
    render_surface &surface, terminalpp::rectangle const &region) const
{
  // If there is no marquee start time, then grab the current time as the
  // starting marquee frame.
  auto const now = pimpl_->animator_.now();

  if (!pimpl_->marquee_start_time_)
  {
    pimpl_->marquee_start_time_ = now;
  }

  assert(pimpl_->marquee_start_time_.has_value());
  auto const frame_1_time =
      *pimpl_->marquee_start_time_ + marquee_delay_time;  // NOLINT
  auto const is_in_frame_0 = now < frame_1_time;
  auto const frame_number =
      is_in_frame_0 ? 0 : ((now - frame_1_time) / marquee_frame_time) + 1;
  auto const character_index = frame_number * characters_per_marquee_frame;

  auto const message_width =
      static_cast<terminalpp::coordinate_type>(pimpl_->message_.size());

  terminalpp::for_each_in_region(
      surface,
      region,
      [this, character_index, message_width](
          terminalpp::element &elem,
          terminalpp::coordinate_type column,  // NOLINT
          terminalpp::coordinate_type row)
      {
        elem = (character_index + column) < message_width
                   ? pimpl_->message_[character_index + column]
                   : ' ';
      });

  if (character_index < message_width)
  {
    auto const next_frame_time =
        is_in_frame_0
            ? frame_1_time
            : frame_1_time + ((frame_number + 1) * marquee_frame_time);

    pimpl_->animator_.redraw_component_at(
        shared_from_this(), {{}, get_size()}, next_frame_time);
  }
}

// ==========================================================================
// MAKE_STATUS_BAR
// ==========================================================================
std::shared_ptr<status_bar> make_status_bar(animator &anim)
{
  return std::make_shared<status_bar>(anim);
}

}  // namespace munin
