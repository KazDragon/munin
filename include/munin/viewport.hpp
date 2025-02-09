#pragma once

#include "munin/basic_component.hpp"

namespace munin {

//* =========================================================================
/// \brief A component that tracks a subset of another, larger component and
/// shows only a section of it.
//* =========================================================================
class MUNIN_EXPORT viewport : public basic_component
{
public:
    //* =====================================================================
    /// \brief Defines a strategy by which a tracked component is allowed
    /// to grow.
    //* =====================================================================
    struct resize_strategy
    {
        //* =================================================================
        /// \brief Destructor
        //* =================================================================
        virtual ~resize_strategy() = default;

        //* =================================================================
        /// \brief Determine the new size of the tracked component given
        /// its preferences and the viewport's current size.
        //* =================================================================
        [[nodiscard]] virtual terminalpp::extent
        calculate_tracked_component_size(
            terminalpp::extent tracked_preferred_size,
            terminalpp::extent viewport_size) const = 0;
    };

    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    explicit viewport(std::shared_ptr<component> tracked_component);

    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    viewport(
        std::shared_ptr<component> tracked_component,
        std::unique_ptr<resize_strategy> strategy);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~viewport() override;

    //* =====================================================================
    /// \brief Returns the anchor bounds of the viewport.
    ///
    /// The anchor bounds are defined as the bounds in which the top-left
    /// corner of the viewport could possibly be in relation to the tracked
    /// component.  For example, if the underlying component is (20,20), and
    /// the viewport was (5,5), then the bounds could by from (0,0)->(15,15)
    /// before running out of space.  In the rectangle returned, the
    /// origin refers to the actual anchor position, and the size refers
    /// to the extent of the bounds as described above.
    //* =====================================================================
    [[nodiscard]] terminalpp::rectangle get_anchor_bounds() const;

    //* =====================================================================
    /// \brief Connect to this signal in order to receive notifications when
    /// the anchor bounds have changed.
    //* =====================================================================
    boost::signals2::signal<void()> on_anchor_bounds_changed;  // NOLINT

private:
    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    void do_set_size(terminalpp::extent const &size) override;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by has_focus().  Derived classes must override this
    /// function in order to return whether this component has focus in a
    /// custom manner.
    //* =====================================================================
    [[nodiscard]] bool do_has_focus() const override;

    //* =====================================================================
    /// \brief Called by set_focus().  Derived classes must override this
    /// function in order to set the focus to this component in a custom
    /// manner.
    //* =====================================================================
    void do_set_focus() override;

    //* =====================================================================
    /// \brief Called by lose_focus().  Derived classes must override this
    /// function in order to lose the focus from this component in a
    /// custom manner.
    //* =====================================================================
    void do_lose_focus() override;

    //* =====================================================================
    /// \brief Called by focus_next().  Derived classes must override this
    /// function in order to move the focus in a custom manner.
    //* =====================================================================
    void do_focus_next() override;

    //* =====================================================================
    /// \brief Called by focus_previous().  Derived classes must override
    /// this function in order to move the focus in a custom manner.
    //* =====================================================================
    void do_focus_previous() override;

    //* =====================================================================
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    [[nodiscard]] bool do_get_cursor_state() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::point do_get_cursor_position() const override;

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed canvas.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param surface the surface on which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    void do_draw(render_surface &surface, terminalpp::rectangle const &region)
        const override;

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &event) override;

    struct impl;
    std::unique_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a strategy where a viewport will grow the tracked
/// component in any direction it desires.
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<viewport::resize_strategy>
make_default_viewport_resize_strategy();

//* =========================================================================
/// \brief Returns a strategy where a viewport will only grow the tracked
/// component in a vertical direction.
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<viewport::resize_strategy>
make_vertical_viewport_resize_strategy();

//* =========================================================================
/// \brief Returns a strategy where a viewport will only grow the tracked
/// component in a horizontal direction.
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<viewport::resize_strategy>
make_horizontal_viewport_resize_strategy();

//* =========================================================================
/// \brief Returns a newly created viewport.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<viewport> make_viewport(
    std::shared_ptr<component> tracked_component);

//* =========================================================================
/// \brief Returns a newly created viewport with a given resize strategy.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<viewport> make_viewport(
    std::shared_ptr<component> tracked_component,
    std::unique_ptr<viewport::resize_strategy> strategy);

}  // namespace munin
