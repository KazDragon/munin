#pragma once

#include "munin/container.hpp"

namespace munin {

//* =========================================================================
/// \brief A default implementation of a container.
//* =========================================================================
class MUNIN_EXPORT basic_container : public container
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    basic_container();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~basic_container();

protected :
    //* =====================================================================
    /// \brief Called by set_position().  Derived classes must override this
    /// function in order to set the position of the component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_position(terminalpp::point const &position) override;

    //* =====================================================================
    /// \brief Called by get_position().  Derived classes must override this
    /// function in order to get the position of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::point do_get_position() const override;

    //* =====================================================================
    /// \brief Called by set_size().  Derived classes must override this
    /// function in order to set the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_size(terminalpp::extent const &size) override;

    //* =====================================================================
    /// \brief Called by get_size().  Derived classes must override this
    /// function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_size() const override;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to get the size of the component in a custom
    /// manner.
    //* =====================================================================
    virtual terminalpp::extent do_get_preferred_size() const override;

    //* =====================================================================
    /// \brief Called by get_number_of_components().  Derived classes must
    /// override this function in order to retrieve the number of components
    /// in this basic_container in a custom manner.
    //* =====================================================================
    virtual u32 do_get_number_of_components() const override;

    //* =====================================================================
    /// \brief Called by add_component().  Derived classes must override
    /// this function in order to add a component to the basic_container in a
    /// custom manner.
    //* =====================================================================
    virtual void do_add_component(
        std::shared_ptr<component> const &component,
        boost::any                 const &hint,
        u32                               layer) override;

    //* ====================================================================
    /// \brief Called by remove_component().  Derived classes must override
    /// this function in order to add a component to the basic_container in a
    /// custom manner.
    //* =====================================================================
    virtual void do_remove_component(
        std::shared_ptr<component> const &component) override;

    //* =====================================================================
    /// \brief Called by get_component().  Derived classes must override this
    /// function in order to retrieve a component in a custom manner.
    //* =====================================================================
    virtual std::shared_ptr<component> do_get_component(
        u32 index) const override;

    //* =====================================================================
    /// \brief Called by get_component_hint().  Derived classes must
    /// override this function in order to retrieve a component hint in a
    /// custom manner.
    //* =====================================================================
    virtual boost::any do_get_component_hint(u32 index) const override;

    //* =====================================================================
    /// \brief Called by get_component_layer().  Derived classes must
    /// override this function in order to retrieve a component layer in a
    /// custom manner.
    //* =====================================================================
    virtual u32 do_get_component_layer(u32 index) const override;

    //* =====================================================================
    /// \brief Called by set_layout.  Derived classes must override this
    /// function in order to set a layout in a custom manner.
    //* =====================================================================
    virtual void do_set_layout(
        std::unique_ptr<munin::layout> lyt,
        u32                            layer) override;

    //* =====================================================================
    /// \brief Called by get_layout.  Derived classes must override this
    /// function in order to get the basic_container's layout in a custom
    /// manner.
    //* =====================================================================
    virtual boost::optional<munin::layout &> do_get_layout(
        u32 layer) const override;

    //* =====================================================================
    /// \brief Called by get_layout_layers.  Derived classes must override
    /// this function in order to get the container's layout in a custom
    /// manner.
    //* =====================================================================
    virtual std::vector<u32> do_get_layout_layers() const override;

    //* =====================================================================
    /// \brief Called by has_focus().  Derived classes must override this
    /// function in order to return whether this component has focus in a
    /// custom manner.
    //* =====================================================================
    virtual bool do_has_focus() const override;

    //* =====================================================================
    /// \brief Called by set_focus().  Derived classes must override this
    /// function in order to set the focus to this component in a custom
    /// manner.
    //* =====================================================================
    virtual void do_set_focus() override;

    //* =====================================================================
    /// \brief Called by lose_focus().  Derived classes must override this
    /// function in order to lose the focus from this component in a
    /// custom manner.
    //* =====================================================================
    virtual void do_lose_focus() override;

    //* =====================================================================
    /// \brief Called by focus_next().  Derived classes must override this
    /// function in order to move the focus in a custom manner.
    //* =====================================================================
    virtual void do_focus_next() override;

    //* =====================================================================
    /// \brief Called by focus_previous().  Derived classes must override
    /// this function in order to move the focus in a custom manner.
    //* =====================================================================
    virtual void do_focus_previous() override;

    //* =====================================================================
    /// \brief Called by enable().  Derived classes must override this
    /// function in order to disable the component in a custom manner.
    //* =====================================================================
    virtual void do_enable() override;

    //* =====================================================================
    /// \brief Called by disable().  Derived classes must override this
    /// function in order to disable the component in a custom manner.
    //* =====================================================================
    virtual void do_disable() override;

    //* =====================================================================
    /// \brief Called by is_enabled().  Derived classes must override this
    /// function in order to return whether the component is disabled or not
    /// in a custom manner.
    //* =====================================================================
    virtual bool do_is_enabled() const override;

    //* =====================================================================
    /// \brief Called by event().  Derived classes must override this
    /// function in order to handle events in a custom manner.
    //* =====================================================================
    virtual void do_event(boost::any const &event) override;

    //* =====================================================================
    /// \brief Called by get_cursor_state().  Derived classes must override
    /// this function in order to return the cursor state in a custom manner.
    //* =====================================================================
    virtual bool do_get_cursor_state() const override;

    //* =====================================================================
    /// \brief Called by get_cursor_position().  Derived classes must
    /// override this function in order to return the cursor position in
    /// a custom manner.
    //* =====================================================================
    virtual terminalpp::point do_get_cursor_position() const override;

    //* =====================================================================
    /// \brief Called by set_cursor_position().  Derived classes must
    /// override this function in order to set the cursor position in
    /// a custom manner.
    //* =====================================================================
    virtual void do_set_cursor_position(terminalpp::point const &position) override;

    //* =====================================================================
    /// \brief Called by layout().  Derived classes must override this
    /// function in order to lay the component out.  If the component
    /// contains subcomponents, these must also be laid out.
    //* =====================================================================
    virtual void do_layout() override;

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

}
