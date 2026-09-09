#include "paint/paint_app.hpp"

#include "paint/paint_canvas.hpp"
#include "paint/paint_model.hpp"
#include "paint/paint_palette.hpp"

#include <munin/compass_layout.hpp>
#include <munin/event_context.hpp>
#include <munin/image.hpp>
#include <terminalpp/virtual_key.hpp>

#include <any>
#include <utility>

namespace paint {
namespace {

using namespace terminalpp::literals;  // NOLINT

}  // namespace

paint_app::paint_app() = default;

paint_app::paint_app(std::shared_ptr<paint_model> model)
  : model_{std::move(model)}
{
    add_paint_components(model_);
}

paint_app::paint_app(std::shared_ptr<munin::component> content)
{
    set_layout(munin::make_compass_layout());
    add_component(std::move(content), munin::compass_layout::heading::centre);
}

paint_app::paint_app(
    std::shared_ptr<munin::component> content,
    std::shared_ptr<paint_model> model)
  : model_{std::move(model)}
{
    set_layout(munin::make_compass_layout());
    add_component(std::move(content), munin::compass_layout::heading::centre);
}

void paint_app::add_paint_components(std::shared_ptr<paint_model> const &model)
{
    set_layout(munin::make_compass_layout());
    add_component(
        std::make_shared<paint_palette>(*model),
        munin::compass_layout::heading::west);
    add_component(
        std::make_shared<paint_canvas>(*model),
        munin::compass_layout::heading::centre);
    add_component(
        munin::make_image("drag paint  q quit"_ets),
        munin::compass_layout::heading::south);
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
    else
    {
        munin::composite_component::do_event(event, context);
    }
}

auto make_paint_app() -> std::shared_ptr<paint_app>
{
    auto model = std::make_shared<paint_model>(terminalpp::extent{0, 0});
    return std::make_shared<paint_app>(model);
}

}  // namespace paint
