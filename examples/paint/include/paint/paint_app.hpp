#pragma once

#include <boost/signals2/signal.hpp>
#include <munin/basic_component.hpp>

#include <memory>

namespace paint {

class paint_app : public munin::basic_component
{
public:
    paint_app();
    explicit paint_app(std::shared_ptr<munin::component> content);

    boost::signals2::signal<void()> on_quit;

protected:
    void do_set_size(terminalpp::extent const &size) override;
    [[nodiscard]] auto do_get_preferred_size() const
        -> terminalpp::extent override;
    [[nodiscard]] auto do_can_receive_focus() const -> bool override;
    void do_draw(
        munin::render_surface &surface,
        terminalpp::rectangle const &region) const override;
    void do_event(std::any const &event) override;
    void do_event(
        std::any const &event, munin::event_context &context) override;

private:
    std::shared_ptr<munin::component> content_;
};

[[nodiscard]] auto make_paint_app() -> std::shared_ptr<paint_app>;

}  // namespace paint
