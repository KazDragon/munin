#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"

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

framed_component::framed_component(
    std::shared_ptr<component> const &frame,
    std::shared_ptr<component> const &inner_component)
{
    add_component(frame);
    add_component(inner_component);
    set_layout(make_framed_component_layout());
}

framed_component::~framed_component()
{
}

std::shared_ptr<framed_component> make_framed_component(
    std::shared_ptr<component> const &frame,
    std::shared_ptr<component> const &inner_component)
{
    return std::make_shared<framed_component>(frame, inner_component);
}

}
