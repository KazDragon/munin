#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include <iostream>

namespace munin {

namespace {

class framed_component_layout : public munin::layout
{
protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any>                 const &hints) const override
    {
        return components[0]->get_preferred_size()
             + components[1]->get_preferred_size();
    }

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    void do_layout(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any>                 const &hints,
        terminalpp::extent                             size) const override
    {
        auto const &frame = components[0];
        frame->set_position({0, 0});
        frame->set_size(size);
        
        auto const &inner = components[1];
        inner->set_position({
            std::min(size.width, 1),
            std::min(size.height, 1)
        });

        inner->set_size({
            size.width == 0 ? 0
          : size.width == 1 ? 1
          : size.width - 2,
            size.height == 0 ? 0
          : size.height == 1 ? 1
          : size.height - 2
        });
    }

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    nlohmann::json do_to_json() const override
    {
        return {};
    }
};

std::unique_ptr<layout> make_framed_component_layout()
{
    return std::unique_ptr<layout>(new framed_component_layout);
}

}

// ==========================================================================
// FRAMED_COMPONENT::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct framed_component::impl
{
    std::shared_ptr<component> inner_component_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
framed_component::framed_component(
    std::shared_ptr<component> const &frame,
    std::shared_ptr<component> const &inner_component)
  : pimpl_(std::make_shared<impl>())
{
    pimpl_->inner_component_ = inner_component;
    
    add_component(frame);
    add_component(inner_component);
    set_layout(make_framed_component_layout());
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
framed_component::~framed_component()
{
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void framed_component::do_event(boost::any const &ev)
{
    std::cout << "framed_component::do_event\n";
    auto *mouse_event = boost::any_cast<terminalpp::ansi::mouse::report>(&ev);
    
    if (mouse_event)
    {
        std::cout << "framed_component::do_event is a mouse event " << *mouse_event << "\n";

        auto inner_mouse_event = *mouse_event;
        
        // Mouse events *always* hit the inner component, and never the
        // frame, so it needs adjusting inward and sending onward instead
        // of being handled by the default event handler, who will likely
        // send the event to the frame instead.
        auto size = get_size();
        std::cout << "framed_component size is " << size << std::endl;
        
        // Correct the mouse event as clicked on this component.
        inner_mouse_event.x_position_ =
          size.width < 2 ? 0 : std::max(mouse_event->x_position_, 1);
        inner_mouse_event.y_position_ =
          size.height < 2 ? 0 : std::max(mouse_event->y_position_, 1);
        
        // Translate mouse co-ordinates to inner-component co-ordinates
        auto inner_position = pimpl_->inner_component_->get_position();
        std::cout << "inner position = " << inner_position << std::endl;
        inner_mouse_event.x_position_ -= inner_position.x;
        inner_mouse_event.y_position_ -= inner_position.y;

        std::cout << "sending " << inner_mouse_event;
        pimpl_->inner_component_->event(inner_mouse_event);
    }
    else
    {
        composite_component::do_event(ev);
    }
}

// ==========================================================================
// MAKE_FRAMED_COMPONENT
// ==========================================================================
std::shared_ptr<framed_component> make_framed_component(
    std::shared_ptr<component> const &frame,
    std::shared_ptr<component> const &inner_component)
{
    return std::make_shared<framed_component>(frame, inner_component);
}

}
