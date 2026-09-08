#pragma once

#include <boost/signals2/signal.hpp>
#include <munin/composite_component.hpp>

#include <memory>

namespace paint {

class paint_model;

class paint_app : public munin::composite_component
{
public:
    paint_app();
    explicit paint_app(std::shared_ptr<paint_model> model);
    explicit paint_app(std::shared_ptr<munin::component> content);
    paint_app(
        std::shared_ptr<munin::component> content,
        std::shared_ptr<paint_model> model);

    boost::signals2::signal<void()> on_quit;

protected:
    void do_event(
        std::any const &event, munin::event_context &context) override;

private:
    void add_paint_components(std::shared_ptr<paint_model> const &model);

    std::shared_ptr<paint_model> model_;
};

[[nodiscard]] auto make_paint_app() -> std::shared_ptr<paint_app>;

}  // namespace paint
