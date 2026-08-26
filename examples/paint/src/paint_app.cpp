#include "paint/paint_app.hpp"

#include <munin/event_context.hpp>
#include <terminalpp/virtual_key.hpp>

#include <any>
#include <utility>

namespace paint {

paint_app::paint_app() = default;

paint_app::paint_app(std::shared_ptr<munin::component> content)
  : content_{std::move(content)}
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
    return std::make_shared<paint_app>();
}

}  // namespace paint
