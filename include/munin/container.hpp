#pragma once

#include "munin/export.hpp"
#include "munin/component.hpp"
#include "munin/core.hpp"
#include <boost/optional.hpp>
#include <vector>

namespace munin {

class layout;

BOOST_STATIC_CONSTANT(
    u32, HIGHEST_LAYER = (std::numeric_limits<u32>::max)());
BOOST_STATIC_CONSTANT(
    u32, LOWEST_LAYER = (std::numeric_limits<u32>::min)());
BOOST_STATIC_CONSTANT(
    u32, DEFAULT_LAYER = (std::numeric_limits<u32>::max)() / 2);

//* =========================================================================
/// \brief A graphical element capable of containing and arranging other
/// subcomponents.
//* =========================================================================
class MUNIN_EXPORT container
    : public component
    , public std::enable_shared_from_this<container>
{
public :
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    container();

    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~container();

    //* =====================================================================
    /// \brief Retrieves the number of components that this container
    /// contains.
    //* =====================================================================
    u32 get_number_of_components() const;

    //* =====================================================================
    /// \brief Adds a component to the container.
    /// \param component The component to add to the container
    /// \param layout_hint A hint to be passed to the container's current
    ///        layout.
    //* =====================================================================
    void add_component(
        std::shared_ptr<component> const &comp,
        boost::any                 const &layout_hint = boost::any(),
        u32                               layer = DEFAULT_LAYER);

    //* =====================================================================
    /// \brief Removes a component from the container.
    //* =====================================================================
    void remove_component(std::shared_ptr<component> const &component);

    //* =====================================================================
    /// \brief Retrieves a component from the container.
    //* =====================================================================
    std::shared_ptr<component> get_component(u32 index) const;

    //* =====================================================================
    /// \brief Retrieves a component's hint from the container.
    //* =====================================================================
    boost::any get_component_hint(u32 index) const;

    //* =====================================================================
    /// \brief Retrieves a component's layer from the container.
    //* =====================================================================
    u32 get_component_layer(u32 index) const;

    //* =====================================================================
    /// \brief Sets the container's current layout for a given layer
    //* =====================================================================
    void set_layout(
        std::unique_ptr<munin::layout> lyt,
        u32                            layer = DEFAULT_LAYER);

    //* =====================================================================
    /// \brief Retrieves the current layout from the container for a given
    /// layer.
    //* =====================================================================
    boost::optional<munin::layout &> get_layout(
        u32 layer = DEFAULT_LAYER) const;

    //* =====================================================================
    /// \brief Returns an array of layers that currently have layouts
    //* =====================================================================
    std::vector<u32> get_layout_layers() const;

protected :
    //* =====================================================================
    /// \brief Called by get_number_of_components().  Derived classes must
    /// override this function in order to retrieve the number of components
    /// in this container in a custom manner.
    //* =====================================================================
    virtual u32 do_get_number_of_components() const = 0;

    //* =====================================================================
    /// \brief Called by add_component().  Derived classes must override
    /// this function in order to add a component to the container in a
    /// custom manner.
    //* =====================================================================
    virtual void do_add_component(
        std::shared_ptr<component> const &comp,
        boost::any                 const &hint,
        u32                               layer) = 0;

    //* =====================================================================
    /// \brief Called by remove_component().  Derived classes must override
    /// this function in order to add a component to the container in a
    /// custom manner.
    //* =====================================================================
    virtual void do_remove_component(
        std::shared_ptr<component> const &comp) = 0;

    //* =====================================================================
    /// \brief Called by get_component().  Derived classes must override this
    /// function in order to retrieve a component in a custom manner.
    //* =====================================================================
    virtual std::shared_ptr<component> do_get_component(
        u32 index) const = 0;

    //* =====================================================================
    /// \brief Called by get_component_hint().  Derived classes must
    /// override this function in order to retrieve a component hint in a
    /// custom manner.
    //* =====================================================================
    virtual boost::any do_get_component_hint(u32 index) const = 0;

    //* =====================================================================
    /// \brief Called by get_component_layer().  Derived classes must
    /// override this function in order to retrieve a component layer in a
    /// custom manner.
    //* =====================================================================
    virtual u32 do_get_component_layer(u32 index) const = 0;

    //* =====================================================================
    /// \brief Called by set_layout.  Derived classes must override this
    /// function in order to set a layout in a custom manner.
    //* =====================================================================
    virtual void do_set_layout(
        std::unique_ptr<munin::layout> lyt
      , u32                      layer) = 0;

    //* =====================================================================
    /// \brief Called by get_layout.  Derived classes must override this
    /// function in order to get the container's layout in a custom manner.
    //* =====================================================================
    virtual boost::optional<munin::layout &> do_get_layout(
        u32 layer) const = 0;

    //* =====================================================================
    /// \brief Called by get_layout_layers.  Derived classes must override
    /// this function in order to get the container's layout in a custom
    /// manner.
    //* =====================================================================
    virtual std::vector<u32> do_get_layout_layers() const = 0;

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

private :
    struct impl;
    std::shared_ptr<impl> pimpl_;
};

}
