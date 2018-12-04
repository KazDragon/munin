#pragma once

#include "munin/composite_component.hpp"
#include "munin/frame.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that models a box that is always completely filled with
/// a given element.
//* =========================================================================
class MUNIN_EXPORT framed_component : public munin::composite_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    framed_component(
        std::shared_ptr<frame> const &outer_frame,
        std::shared_ptr<component> const &inner_component);

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    ~framed_component() override;

protected :
    //* =====================================================================
    /// \brief Called by has_focus().  Derived classes must override this
    /// function in order to return whether this component has focus in a
    /// custom manner.
    //* =====================================================================
    bool do_has_focus() const override;

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
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    void do_event(boost::any const &ev) override;
    
    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    nlohmann::json do_to_json() const override;

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created filled box.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<framed_component> make_framed_component(
    std::shared_ptr<frame> const &outer_frame,
    std::shared_ptr<component> const &inner_component);

}
