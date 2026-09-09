#pragma once

#include <munin/basic_component.hpp>

namespace paint {

class paint_model;

class paint_palette : public munin::basic_component
{
public:
    explicit paint_palette(paint_model &model);

protected:
    [[nodiscard]] auto do_get_preferred_size() const
        -> terminalpp::extent override;
    [[nodiscard]] auto do_can_receive_focus() const -> bool override;
    void do_draw(
        munin::render_surface &surface,
        terminalpp::rectangle const &region) const override;
    void do_event(
        std::any const &event, munin::event_context &context) override;

private:
    paint_model &model_;
};

}  // namespace paint
