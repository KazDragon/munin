#include "paint/paint_app.hpp"

#include "paint/paint_canvas.hpp"
#include "paint/paint_model.hpp"
#include "paint/paint_palette.hpp"

#include <munin/compass_layout.hpp>
#include <munin/event_context.hpp>
#include <munin/filled_box.hpp>
#include <munin/image.hpp>
#include <munin/view.hpp>
#include <terminalpp/virtual_key.hpp>

#include <any>
#include <utility>

namespace paint {
namespace {

using namespace terminalpp::literals;  // NOLINT

[[nodiscard]] auto make_content(std::shared_ptr<paint_model> const &model)
    -> std::shared_ptr<munin::component>
{
    return munin::view(
        munin::make_compass_layout(),
        munin::make_fill(' '),
        munin::compass_layout::heading::centre,
        std::make_shared<paint_palette>(*model),
        munin::compass_layout::heading::west,
        std::make_shared<paint_canvas>(*model),
        munin::compass_layout::heading::centre,
        munin::make_image("drag paint  q quit"_ets),
        munin::compass_layout::heading::south);
}

}  // namespace

paint_app::paint_app() = default;

paint_app::paint_app(std::shared_ptr<munin::component> content)
  : content_{std::move(content)}
{
}

paint_app::paint_app(
    std::shared_ptr<munin::component> content,
    std::shared_ptr<paint_model> model)
  : content_{std::move(content)}, model_{std::move(model)}
{
}

void paint_app::do_set_size(terminalpp::extent const &size)
{
    munin::basic_component::do_set_size(size);

    if (content_)
    {
        content_->set_size(size);
    }
}

auto paint_app::do_get_preferred_size() const -> terminalpp::extent
{
    return content_ ? content_->get_preferred_size() : terminalpp::extent{};
}

auto paint_app::do_can_receive_focus() const -> bool
{
    return false;
}

void paint_app::do_draw(
    munin::render_surface &surface, terminalpp::rectangle const &region) const
{
    if (content_)
    {
        content_->draw(surface, region);
    }
}

void paint_app::do_event(std::any const &event)
{
    if (auto const *vk = std::any_cast<terminalpp::virtual_key>(&event);
        vk != nullptr
        && (vk->key == terminalpp::vk::lowercase_q
            || vk->key == terminalpp::vk::uppercase_q))
    {
        on_quit();
    }
    else if (content_)
    {
        content_->event(event);
    }
}

void paint_app::do_event(std::any const &event, munin::event_context &context)
{
    if (auto const *vk = std::any_cast<terminalpp::virtual_key>(&event);
        vk != nullptr
        && (vk->key == terminalpp::vk::lowercase_q
            || vk->key == terminalpp::vk::uppercase_q))
    {
        on_quit();
    }
    else if (content_)
    {
        content_->event(event, context);
    }
}

auto make_paint_app() -> std::shared_ptr<paint_app>
{
    auto model = std::make_shared<paint_model>(terminalpp::extent{0, 0});
    return std::make_shared<paint_app>(make_content(model), model);
}

}  // namespace paint
