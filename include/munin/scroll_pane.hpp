#pragma once
#include "munin/framed_component.hpp"
#include <memory>

namespace munin {

//* =========================================================================
/// \brief A component that wraps another component in a frame,
/// and implements scroll bars that track the visible section of the 
/// component.
//* =========================================================================
class MUNIN_EXPORT scroll_pane : public framed_component
{
public:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    scroll_pane(std::shared_ptr<component> const &inner_component);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~scroll_pane();

    //* =====================================================================
    /// \brief Sets the attribute of the elements when the inner component
    /// has focus.
    //* =====================================================================
    void set_highlight_attribute(terminalpp::attribute const &attr);

    //* =====================================================================
    /// \brief Sets the attribute of the elements when the inner component
    /// does not have focus.
    //* =====================================================================
    void set_lowlight_attribute(terminalpp::attribute const &attr);

private:
    scroll_pane(
        std::shared_ptr<component> const &frame,
        std::shared_ptr<component> const &viewport,
        std::shared_ptr<component> const &inner_component);

    struct impl;
    std::unique_ptr<impl> pimpl_;
};

MUNIN_EXPORT
std::shared_ptr<scroll_pane> make_scroll_pane(std::shared_ptr<component> const &inner_component);

}
