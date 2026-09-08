#pragma once

#include <boost/signals2/signal.hpp>
#include <munin/basic_component.hpp>

#include <memory>

namespace paint {

class paint_model;

class paint_app : public munin::basic_component
{
public:
    paint_app();
    explicit paint_app(std::shared_ptr<munin::component> content);
    paint_app(
        std::shared_ptr<munin::component> content,
        std::shared_ptr<paint_model> model);

    boost::signals2::signal<void()> on_quit;

protected:
    void do_set_size(terminalpp::extent const &size) override;
    [[nodiscard]] auto do_get_preferred_size() const
        -> terminalpp::extent override;
    [[nodiscard]] auto do_has_focus() const -> bool override;
    void do_set_focus() override;
    void do_lose_focus() override;
    void do_focus_next() override;
    void do_focus_previous() override;
    void do_draw(
        munin::render_surface &surface,
        terminalpp::rectangle const &region) const override;
    void do_event(
        std::any const &event, munin::event_context &context) override;

private:
    std::shared_ptr<munin::component> content_;
    std::shared_ptr<paint_model> model_;
};

[[nodiscard]] auto make_paint_app() -> std::shared_ptr<paint_app>;

}  // namespace paint
