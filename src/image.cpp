#include "munin/image.hpp"
#include "munin/detail/json_adaptors.hpp"
#include "munin/render_surface.hpp"
#include <boost/make_unique.hpp>
#include <algorithm>
#include <utility>

using namespace terminalpp::literals;  // NOLINT

namespace munin {

// ==========================================================================
// BRUSH::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct image::impl
{
  std::vector<terminalpp::string> content;
  terminalpp::element fill;
  bool can_receive_focus{false};
};

// ==========================================================================
// GET_CONTENT_BASIS
// ==========================================================================
static terminalpp::point get_content_basis(
    terminalpp::extent const &component_size,
    terminalpp::extent const &content_size)
{
  return {
      (component_size.width_ - content_size.width_) / 2,
      (component_size.height_ - content_size.height_) / 2};
}

// ==========================================================================
// GET_CONTENT_EXTENT
// ==========================================================================
static terminalpp::extent get_content_extent(
    terminalpp::extent const &component_size,
    terminalpp::extent const &content_size)
{
  return {
      (std::min)(content_size.width_, component_size.width_),
      (std::min)(content_size.height_, component_size.height_)};
}

// ==========================================================================
// GET_CONTENT_BOUNDS
// ==========================================================================
static terminalpp::rectangle get_content_bounds(
    terminalpp::extent const &component_size,
    terminalpp::extent const &content_size)
{
  return {
      get_content_basis(component_size, content_size),
      get_content_extent(component_size, content_size)};
}

// ==========================================================================
// HAS_ZERO_DIMENSION
// ==========================================================================
static bool has_zero_dimension(terminalpp::rectangle const &bounds)
{
  return bounds.size_.width_ == 0 || bounds.size_.height_ == 0;
}

// ==========================================================================
// ADD_REDRAW_REGION
// ==========================================================================
static void add_redraw_region(
    std::vector<terminalpp::rectangle> &redraw_regions,
    terminalpp::extent const &component_size,
    terminalpp::extent const &content_size)
{
  if (auto const content_bounds =
          get_content_bounds(component_size, content_size);
      !has_zero_dimension(content_bounds))
  {
    redraw_regions.push_back(content_bounds);
  }
}

// ==========================================================================
// DRAW_FILL_LINE
// ==========================================================================
static void draw_fill_line(
    render_surface &surface,
    terminalpp::point const &origin,
    terminalpp::coordinate_type const &width,
    terminalpp::element const &fill)
{
  for (terminalpp::coordinate_type column = origin.x_;
       column < origin.x_ + width;
       ++column)
  {
    surface[column][origin.y_] = fill;
  }
}

// ==========================================================================
// DRAW_CONTENT_LINE
// ==========================================================================
static void draw_content_line(
    render_surface &surface,
    terminalpp::point const &origin,
    terminalpp::coordinate_type const &content_start,  // NOLINT
    terminalpp::coordinate_type const &line_width,
    terminalpp::string const &content,
    terminalpp::element const &fill)
{
  for (auto column = origin.x_; column < origin.x_ + line_width; ++column)
  {
    bool const column_has_content =
        column >= content_start && column < content_start + content.size();

    surface[column][origin.y_] =
        column_has_content ? content[column - content_start] : fill;
  }
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(terminalpp::element fill) : image("", fill)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(terminalpp::string content, terminalpp::element fill)
  : image(std::vector<terminalpp::string>{std::move(content)}, fill)
{
}

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
image::image(std::vector<terminalpp::string> content, terminalpp::element fill)
  : pimpl_(boost::make_unique<impl>())
{
  // There is a special case for "empty" content, where the content is
  // a single empty string.  In this case, it is not stored, and is as if
  // there were no content at all.
  if (!content.empty() && (content.size() != 1 || !content[0].empty()))
  {
    pimpl_->content = content;
  }

  pimpl_->fill = fill;
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
image::~image() = default;

// ==========================================================================
// SET_FILL
// ==========================================================================
void image::set_fill(terminalpp::element const &fill)
{
  pimpl_->fill = fill;
  on_redraw({{{0, 0}, get_size()}});
}

// ==========================================================================
// SET_CONTENT
// ==========================================================================
void image::set_content()
{
  auto const old_content_bounds{
      get_content_bounds(get_size(), get_preferred_size())};

  pimpl_->content.clear();

  if (!has_zero_dimension(old_content_bounds))
  {
    on_preferred_size_changed();
    on_redraw({old_content_bounds});
  }
}

// ==========================================================================
// SET_CONTENT
// ==========================================================================
void image::set_content(terminalpp::string const &content)
{
  set_content(std::vector<terminalpp::string>{content});
}

// ==========================================================================
// SET_CONTENT
// ==========================================================================
void image::set_content(std::vector<terminalpp::string> const &content)
{
  // Special cases: setting content to an empty vector or a vector
  // of an empty string is equivalent to setting an empty content.
  if (content.empty() || (content.size() == 1U && content[0].empty()))
  {
    set_content();
    return;
  }

  std::vector<terminalpp::rectangle> redraw_regions;
  auto const size = get_size();

  add_redraw_region(redraw_regions, size, get_preferred_size());
  pimpl_->content = content;
  add_redraw_region(redraw_regions, size, get_preferred_size());

  on_preferred_size_changed();
  on_redraw(redraw_regions);
}

// ==========================================================================
// SET_CAN_RECEIVE_FOCUS
// ==========================================================================
void image::set_can_receive_focus(bool can_receive_focus)
{
  pimpl_->can_receive_focus = can_receive_focus;
}

// ==========================================================================
// DO_CAN_RECEIVE_FOCUS
// ==========================================================================
bool image::do_can_receive_focus() const
{
  return pimpl_->can_receive_focus;
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent image::do_get_preferred_size() const
{
  return pimpl_->content.empty()
             ? terminalpp::extent()
             : terminalpp::extent(
                 static_cast<terminalpp::coordinate_type>(
                     std::max_element(
                         pimpl_->content.begin(),
                         pimpl_->content.end(),
                         [](auto const &lhs, auto const &rhs)
                         { return lhs.size() < rhs.size(); })
                         ->size()),
                 static_cast<terminalpp::coordinate_type>(
                     pimpl_->content.size()));
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void image::do_draw(
    render_surface &surface, terminalpp::rectangle const &region) const
{
  auto const size = get_size();
  auto const content_size = get_preferred_size();
  auto const content_basis = get_content_basis(size, content_size);

  for (terminalpp::coordinate_type row = region.origin_.y_;
       row < region.origin_.y_ + region.size_.height_;
       ++row)
  {
    bool const row_has_content =
        row >= content_basis.y_
        && row < content_basis.y_ + pimpl_->content.size();

    if (row_has_content)
    {
      draw_content_line(
          surface,
          {region.origin_.x_, row},
          content_basis.x_,
          region.size_.width_,
          pimpl_->content[row - content_basis.y_],
          pimpl_->fill);
    }
    else
    {
      draw_fill_line(
          surface, {region.origin_.x_, row}, region.size_.width_, pimpl_->fill);
    }
  }
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json image::do_to_json() const
{
  nlohmann::json patch = R"([
        { "op": "replace", "path": "/type", "value": "image" }
    ])"_json;

  auto json = basic_component::do_to_json().patch(patch);

  json["fill"] = detail::to_json(pimpl_->fill);
  json["content"]["size"] = pimpl_->content.size();

  for (size_t index = 0; index < pimpl_->content.size(); ++index)
  {
    json["content"]["content"][index] =
        terminalpp::to_string(pimpl_->content[index]);
  }

  return json;
}

// ==========================================================================
// MAKE_IMAGE
// ==========================================================================
std::shared_ptr<image> make_image(terminalpp::element fill)
{
  return std::make_shared<image>(std::move(fill));
}

// ==========================================================================
// MAKE_IMAGE
// ==========================================================================
std::shared_ptr<image> make_image(
    terminalpp::string content, terminalpp::element fill)
{
  return std::make_shared<image>(std::move(content), std::move(fill));
}

// ==========================================================================
// MAKE_IMAGE
// ==========================================================================
std::shared_ptr<image> make_image(
    std::vector<terminalpp::string> content, terminalpp::element fill)
{
  return std::make_shared<image>(std::move(content), std::move(fill));
}

}  // namespace munin
