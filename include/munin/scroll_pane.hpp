#pragma once
#include "munin/framed_component.hpp"
#include "munin/viewport.hpp"

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
    explicit scroll_pane(std::shared_ptr<component> const &inner_component);

    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    scroll_pane(
        std::shared_ptr<component> const &inner_component,
        std::unique_ptr<viewport::resize_strategy> viewport_resize_strategy);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~scroll_pane() override;

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

//* =========================================================================
/// \brief Makes a new scroll pane around a given component.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<scroll_pane> make_scroll_pane(
    std::shared_ptr<component> const &inner_component);

//* =========================================================================
/// \brief Makes a new scroll around a given component with the specified
/// resize strategy.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<scroll_pane> make_scroll_pane(
    std::shared_ptr<component> const &inner_component,
    std::unique_ptr<viewport::resize_strategy> strategy);

}  // namespace munin
