#pragma once

#include "munin/export.hpp"
#include "munin/basic_component.hpp"
#include "odin/core.hpp"
#include <boost/optional.hpp>

namespace munin {

//* =========================================================================
/// \brief A class that models a horizontal scroll bar.
//* =========================================================================
class MUNIN_EXPORT horizontal_scroll_bar : public basic_component
{
public :
    //* =====================================================================
    /// \brief Constructor
    /// \param elements - a multidimentional array with each element
    /// representing one horizontal line of the horizontal_scroll_bar.
    //* =====================================================================
    horizontal_scroll_bar();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~horizontal_scroll_bar();

    //* =====================================================================
    /// \fn on_page_left
    /// \brief Connect to this signal in order to be informed of a control
    /// that causes the scrollbar to want to be paged left.
    //* =====================================================================
    boost::signal<
        void()
    > on_page_left;

    //* =====================================================================
    /// \fn on_page_right
    /// \brief Connect to this signal in order to be informed of a control
    /// that causes the scrollbar to want to be paged right.
    //* =====================================================================
    boost::signal<
        void()
    > on_page_right;

    //* =====================================================================
    /// \brief Gets the position that the slider is in.  This is expressed
    /// as an optional percentage where 0 is the absolute left, 100 is the
    /// absolute right, and 1..99 is somewhere inbetween.  No percentage
    /// (the default) indicates that the slider is not be present.
    //* =====================================================================
    boost::optional<odin::u8> get_slider_position() const;

    //* =====================================================================
    /// \brief Sets the position that the slider is in.  This is expressed
    /// as an optional percentage where 0 is the absolute left, 100 is the
    /// absolute right, and 1..99 is somewhere inbetween.  No percentage
    /// (the default) indicates that the slider should not be present.
    //* =====================================================================
    void set_slider_position(boost::optional<odin::u8> percentage);

protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size() const;

    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_size(terminalpp::extent const &size);

    //* =====================================================================
    /// \brief Called by draw().  Derived classes must override this function
    /// in order to draw onto the passed context.  A component must only draw
    /// the part of itself specified by the region.
    ///
    /// \param ctx the context in which the component should draw itself.
    /// \param region the region relative to this component's origin that
    /// should be drawn.
    //* =====================================================================
    virtual void do_draw(
        context         &ctx
      , rectangle const &region);

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event);

    //* =====================================================================
    /// \brief Called by set_attribute().  Derived classes must override this
    /// function in order to set an attribute in a custom manner.
    //* =====================================================================
    virtual void do_set_attribute(
        std::string const &name, boost::any const &attr);

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

//* =========================================================================
/// \brief Returns a newly created horizontal scroll bar
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<horizontal_scroll_bar> make_horizontal_scroll_bar();

}
