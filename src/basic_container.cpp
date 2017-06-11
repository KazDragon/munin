#include "munin/basic_container.hpp"
#include "munin/layout.hpp"
#include <terminalpp/ansi/mouse.hpp>
#include <boost/scope_exit.hpp>

namespace munin {

namespace {
    typedef std::map<u32, std::unique_ptr<layout>> layered_layout_map;
}

// ==========================================================================
// BASIC_CONTAINER::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct basic_container::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl(basic_container &self)
      : self_(self),
        cursor_state_(false),
        enabled_(true)
    {
    }

    // ======================================================================
    // SUBCOMPONENT_FOCUS_SET_HANDLER
    // ======================================================================
    void subcomponent_focus_set_handler(
        std::weak_ptr<component> const &subcomponent)
    {
        auto focussed_component = subcomponent.lock();

        if (focussed_component != NULL)
        {
            // Iterate through all our subcomponents and ensure that this is
            // the only subcomponent to have focus.  Then, if we do not have
            // focus, set our focus.
            for (auto const &current_component : components_)
            {
                if (current_component != focussed_component
                 && current_component->has_focus())
                {
                    current_component->lose_focus();
                }
            }

            if (!has_focus_)
            {
                has_focus_ = true;
                self_.on_focus_set();
            }

            focus_changed();
        }
    }

    // ======================================================================
    // SUBCOMPONENT_FOCUS_LOST_HANDLER
    // ======================================================================
    void subcomponent_focus_lost_handler(
        std::weak_ptr<component> const &subcomponent)
    {
        auto unfocussed_component = subcomponent.lock();

        if (unfocussed_component != NULL)
        {
            // Iterate through all our subcomponents to see if we have any
            // subcomponents focused (for example, setting the focus of a
            // new subcomponent may cause the focus for the reporting
            // component to be lost).  If we do not, then unfocus this
            // component.
            bool component_is_focused = false;

            for (auto const &current_component : components_)
            {
                if (current_component->has_focus())
                {
                    component_is_focused = true;
                    break;
                }
            }

            if (!component_is_focused)
            {
                has_focus_ = false;
                self_.on_focus_lost();
            }

            focus_changed();
        }
    }

    // ======================================================================
    // FOCUS_CHANGED
    // ======================================================================
    void focus_changed()
    {
        if (has_focus_)
        {
            // Find the currently focussed subcomponent.
            for (auto const &current_component : components_)
            {
                if (current_component->has_focus())
                {
                    // Update for the new cursor state if necessary.
                    bool new_cursor_state =
                        current_component->get_cursor_state();

                    if (new_cursor_state != cursor_state_)
                    {
                        cursor_state_ = new_cursor_state;
                        self_.on_cursor_state_changed(cursor_state_);
                    }

                    // If the cursor state is enabled, then update for a new
                    // cursor position.
                    if (cursor_state_)
                    {
                        terminalpp::point cursor_position =
                            current_component->get_position()
                          + current_component->get_cursor_position();

                        self_.on_cursor_position_changed(cursor_position);
                    }
                }
            }
        }
    }

    // ======================================================================
    // SUBCOMPONENT_CURSOR_STATE_CHANGE_HANDLER
    // ======================================================================
    void subcomponent_cursor_state_change_handler(
        std::weak_ptr<component> const &weak_subcomponent
      , bool                            state)
    {
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent
         && state != cursor_state_
         && subcomponent->has_focus())
        {
            cursor_state_ = state;
            self_.on_cursor_state_changed(cursor_state_);
        }
    }

    // ======================================================================
    // SUBCOMPONENT_CURSOR_POSITION_CHANGE_HANDLER
    // ======================================================================
    void subcomponent_cursor_position_change_handler(
        std::weak_ptr<component> const &weak_subcomponent
      , terminalpp::point               position)
    {
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent && subcomponent->has_focus())
        {
            self_.on_cursor_position_changed(
                subcomponent->get_position() + position);
        }
    }

    // ======================================================================
    // SUBCOMPONENT_POSITION_CHANGE_HANDLER
    // ======================================================================
    void subcomponent_position_change_handler(
        std::weak_ptr<component> const &weak_subcomponent
      , terminalpp::point               changed_from
      , terminalpp::point               changed_to)
    {
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent)
        {
            auto const subcomponent_size = subcomponent->get_size();

            std::vector<rectangle> regions =
            {
                rectangle(changed_from, subcomponent_size),
                rectangle(changed_to, subcomponent_size)
            };

            self_.on_redraw(
            {
                { changed_from, subcomponent_size },
                { changed_to, subcomponent_size },
            });
        }
    }

    // ======================================================================
    // SUBCOMPONENT_PREFERRED_SIZE_CHANGE_HANDLER
    // ======================================================================
    void subcomponent_preferred_size_changed_handler(
        std::weak_ptr<component> const &weak_subcomponent)
    {
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent)
        {
            self_.on_preferred_size_changed();
        }
    }

    // ======================================================================
    // FOCUS_NEXT_HAS_FOCUS
    // ======================================================================
    void focus_next_has_focus()
    {
        // We have focus.  Therefore, find the subcomponent that also has
        // focus and tell it to move to the next focus.  After that, if it
        // has no focus, we move the focus to the next subcomponent that
        // can be focused.  If there is no such object, then we lose our
        // focus.
        u32 number_of_components = u32(components_.size());

        for (u32 index = 0; index < number_of_components; ++index)
        {
            auto current_component = components_[index];

            if (current_component->has_focus())
            {
                current_component->focus_next();

                if (!current_component->has_focus())
                {
                    bool focused = false;

                    // Find the index of the next component that can have focus.
                    for (u32 next_index = index + 1;
                         next_index < number_of_components && !focused;
                         ++next_index)
                    {
                        auto next_component = components_[next_index];

                        if (next_component->is_enabled())
                        {
                            next_component->focus_next();
                            focused = true;
                        }
                    }

                    if (!focused)
                    {
                        has_focus_ = false;
                        self_.on_focus_lost();
                    }
                }

                break;
            }
        }
    }

    // ======================================================================
    // FOCUS_NEXT_NO_FOCUS
    // ======================================================================
    void focus_next_no_focus()
    {
        // We do not currently have any focus, therefore we command the first
        // subcomponent to focus its next component.
        for (auto const &current_component : components_)
        {
            if (current_component->is_enabled())
            {
                current_component->focus_next();
                break;
            }
        }
    }

    // ======================================================================
    // FOCUS_PREVIOUS_HAS_FOCUS
    // ======================================================================
    void focus_previous_has_focus()
    {
        // We do have focus.  Find the currently focused component and tell
        // it to move its focus to its previous subcomponent.  If, after
        // that, it no longer has focus, then we find the component prior
        // to that that can have focus and tell it to set its previous focus.
        // If there is no component to focus, then just lose our focus.
        auto number_of_components = u32(components_.size());

        for (u32 index = 0; index < number_of_components; ++index)
        {
            auto current_component = components_[index];

            if (current_component->has_focus())
            {
                current_component->focus_previous();

                if (!current_component->has_focus())
                {
                    bool focused = false;

                    // Seek for the first subcomponent prior to the unfocused
                    // one that can have a focus, and focus it.
                    for (auto last_index = index;
                         last_index > 0 && !focused;
                         --last_index)
                    {
                        auto last_component = components_[last_index - 1];

                        if (last_component->is_enabled())
                        {
                            // If the component is a container, this will focus
                            // the last component in that container.
                            last_component->focus_previous();
                            focused = true;
                        }
                    }

                    if (!focused)
                    {
                        // There are no subcomponents to focus.
                        has_focus_ = false;
                        self_.on_focus_lost();
                    }
                }

                break;
            }
        }
    }

    // ======================================================================
    // FOCUS_PREVIOUS_NO_FOCUS
    // ======================================================================
    void focus_previous_no_focus()
    {
        // Find the last component that could have focus and focus its
        // last element.
        auto number_of_components = u32(components_.size());

        for (auto index = number_of_components; index > 0; --index)
        {
            auto last_component = components_[index - 1];

            if (last_component->is_enabled())
            {
                last_component->focus_previous();
                break;
            }
        }
    }

    basic_container                                      &self_;
    std::weak_ptr<component>                              parent_;
    std::vector<std::shared_ptr<component>>               components_;
    std::vector<boost::any>                               component_hints_;
    std::vector<u32>                                      component_layers_;
    std::vector<std::vector<boost::signals2::connection>> component_connections_;
    layered_layout_map                                    layouts_;
    rectangle                                             bounds_;
    bool                                                  has_focus_;
    bool                                                  cursor_state_;
    bool                                                  enabled_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
basic_container::basic_container()
{
    pimpl_ = std::make_shared<impl>(std::ref(*this));
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
basic_container::~basic_container()
{
    for (auto &connections : pimpl_->component_connections_)
    {
        for (auto &connection : connections)
        {
            connection.disconnect();
        }
    }
}


// ==========================================================================
// DO_GET_NUMBER_OF_COMPONENTS
// ==========================================================================
u32 basic_container::do_get_number_of_components() const
{
    return u32(pimpl_->components_.size());
}

// ==========================================================================
// DO_ADD_COMPONENT
// ==========================================================================
void basic_container::do_add_component(
    std::shared_ptr<component> const &comp
  , boost::any                 const &hint
  , u32                         layer)
{
    // Store the component and the layer in which it is to be drawn.
    pimpl_->components_.push_back(comp);
    pimpl_->component_hints_.push_back(hint);
    pimpl_->component_layers_.push_back(layer);

    std::vector<boost::signals2::connection> component_connections;

    // Register for callbacks for when the new subcomponent either gains
    // or loses focus.  We can make sure our own focus is correct based
    // on this information.
    component_connections.push_back(comp->on_focus_set.connect(
        std::bind(
            &basic_container::impl::subcomponent_focus_set_handler
          , pimpl_
          , std::weak_ptr<component>(comp))));

    component_connections.push_back(comp->on_focus_lost.connect(
        std::bind(
            &basic_container::impl::subcomponent_focus_lost_handler
          , pimpl_
          , std::weak_ptr<component>(comp))));

    // Register for callbacks for when the subcomponent's cursor state
    // or position changes.
    component_connections.push_back(comp->on_cursor_state_changed.connect(
        std::bind(
            &basic_container::impl::subcomponent_cursor_state_change_handler
          , pimpl_
          , std::weak_ptr<component>(comp)
          , std::placeholders::_1)));

    component_connections.push_back(comp->on_cursor_position_changed.connect(
        std::bind(
            &basic_container::impl::subcomponent_cursor_position_change_handler
          , pimpl_
          , std::weak_ptr<component>(comp)
          , std::placeholders::_1)));

    // Register for callbacks for when the subcomponent's preferred size
    // changes.
    component_connections.push_back(comp->on_preferred_size_changed.connect(
        std::bind(
            &basic_container::impl::subcomponent_preferred_size_changed_handler
           , pimpl_
           , std::weak_ptr<component>(comp))));

    pimpl_->component_connections_.push_back(component_connections);
}

// ==========================================================================
// DO_REMOVE_COMPONENT
// ==========================================================================
void basic_container::do_remove_component(
    std::shared_ptr<component> const &comp)
{
    // When we remove a component, we must also remove the associated hint,
    // layer, and signal connections (which must also be disconnected).
    auto current_component = pimpl_->components_.begin();
    auto current_hint =      pimpl_->component_hints_.begin();
    auto current_layer =     pimpl_->component_layers_.begin();
    auto current_signals =   pimpl_->component_connections_.begin();

    while (current_component != pimpl_->components_.end()
        && current_hint      != pimpl_->component_hints_.end()
        && current_layer     != pimpl_->component_layers_.end()
        && current_signals   != pimpl_->component_connections_.end())
    {
        if (*current_component == comp)
        {
            current_component  = pimpl_->components_.erase(current_component);
            current_hint       = pimpl_->component_hints_.erase(current_hint);
            current_layer      = pimpl_->component_layers_.erase(current_layer);

            for (auto &sig : *current_signals)
            {
                sig.disconnect();
            }

            current_signals = pimpl_->component_connections_.erase(current_signals);
        }
        else
        {
            ++current_component;
            ++current_hint;
            ++current_layer;
            ++current_signals;
        }
    }
}

// ==========================================================================
// DO_GET_COMPONENT
// ==========================================================================
std::shared_ptr<component> basic_container::do_get_component(u32 index) const
{
    return pimpl_->components_[index];
}

// ==========================================================================
// DO_GET_COMPONENT_HINT
// ==========================================================================
boost::any basic_container::do_get_component_hint(u32 index) const
{
    return pimpl_->component_hints_[index];
}

// ==========================================================================
// DO_GET_COMPONENT_LAYER
// ==========================================================================
u32 basic_container::do_get_component_layer(u32 index) const
{
    return pimpl_->component_layers_[index];
}

// ==========================================================================
// DO_SET_LAYOUT
// ==========================================================================
void basic_container::do_set_layout(
    std::unique_ptr<munin::layout> lyt
  , u32                      layer)
{
    pimpl_->layouts_[layer] = std::move(lyt);
}

// ==========================================================================
// DO_GET_LAYOUT
// ==========================================================================
boost::optional<layout &> basic_container::do_get_layout(u32 layer) const
{
    auto result = pimpl_->layouts_.find(layer);

    return result == pimpl_->layouts_.end()
                   ? boost::optional<munin::layout &>()
                   : boost::optional<munin::layout &>(*result->second.get());
}

// ==========================================================================
// DO_GET_LAYOUT_LAYERS
// ==========================================================================
std::vector<u32> basic_container::do_get_layout_layers() const
{
    std::vector<u32> layers(pimpl_->layouts_.size());

    u32 index = 0;
    for (auto const &lp : pimpl_->layouts_)
    {
        layers[index++] = lp.first;
    }

    return layers;
}

}

