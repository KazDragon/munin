#include "munin/container.hpp"
#include "munin/algorithm.hpp"
#include "munin/context.hpp"
#include "munin/layout.hpp"
#include "munin/rectangle.hpp"
#include <terminalpp/canvas_view.hpp>
#include <boost/scope_exit.hpp>
#include <vector>

namespace munin {

namespace {
    using component_connections_type =
        std::pair<
            std::shared_ptr<component>,
            std::vector<boost::signals2::connection>>;
}

// ==========================================================================
// CONTAINER::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct container::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl(container &self)
    {
    }

    // ======================================================================
    // LAYOUT_CONTAINER
    // ======================================================================
    void layout_container()
    {
        if (layout_)
        {
            (*layout_)(components_, {}, {});
        }
    }
    
    // ======================================================================
    // SUBCOMPONENT_REDRAW_HANDLER
    // ======================================================================
    void subcomponent_redraw_handler(
        std::weak_ptr<component> weak_subcomponent,
        std::vector<rectangle>   regions)
    {
        /*
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent != NULL)
        {
            // Each region is bound to the origin of the component in question.
            // It must be rebound to the origin of the container.  We do this
            // by offsetting the regions' origins by the origin of the
            // subcomponent within this container.
            auto origin = subcomponent->get_position();

            for (auto &rect : regions)
            {
                rect.origin.x += origin.x;
                rect.origin.y += origin.y;
            }

            // This new information must be passed up the component heirarchy.
            self_.on_redraw(regions);
        }
        */
    }

    // ======================================================================
    // SUBCOMPONENT_CURSOR_POSITION_CHANGE_HANDLER
    // ======================================================================
    void subcomponent_cursor_position_change_handler(
        std::weak_ptr<component> weak_subcomponent
      , terminalpp::point        position)
    {
        /*
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent != NULL && subcomponent->has_focus())
        {
            self_.on_cursor_position_changed(self_.get_position() + position);
        }
        */
    }

    // ======================================================================
    // ENSURE_COMPONENTS_SORTED
    // ======================================================================
    void ensure_components_sorted() const
    {
        /*
        if (dirty_)
        {
            components_.clear();
            std::vector<u32> layers;

            auto number_of_components = self_.get_number_of_components();

            for (u32 index = 0; index < number_of_components; ++index)
            {
                auto comp       = self_.get_component(index);
                auto comp_layer = self_.get_component_layer(index);

                auto component_insert_position = components_.begin();
                auto layer_insert_position =     layers.begin();

                while (component_insert_position != components_.end()
                    && layer_insert_position != layers.end()
                    && comp_layer >= *layer_insert_position)
                {
                    ++component_insert_position;
                    ++layer_insert_position;
                }

                components_.insert(component_insert_position, comp);
                layers.insert(layer_insert_position, comp_layer);
            }

            dirty_ = false;
        }
        */
    }

    munin::rectangle                        bounds_;
    std::unique_ptr<munin::layout>          layout_;
    std::vector<std::shared_ptr<component>> components_;
    
    /*
    container                               &self_;
    bool                                     dirty_ = true;
    std::vector<std::shared_ptr<component>>  components_;
    std::vector<component_connections_type>  component_connections_;
    */
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
container::container()
{
    pimpl_ = std::make_shared<impl>(ref(*this));
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
container::~container()
{
    /*
    for(auto &con : pimpl_->component_connections_)
    {
        for (auto &cnx : con.second)
        {
            cnx.disconnect();
        }
    }
    */
}

// ==========================================================================
// SET_LAYOUT
// ==========================================================================
void container::set_layout(
    std::unique_ptr<munin::layout> &&lyt
  , u32                              layer /*= DEFAULT_LAYER*/)
{
    pimpl_->layout_ = std::move(lyt);
    
    if (!pimpl_->components_.empty())
    {
        pimpl_->layout_container();
    }
}

// ==========================================================================
// ADD_COMPONENT
// ==========================================================================
void container::add_component(
    std::shared_ptr<component> const &comp
  , boost::any                 const &layout_hint
  , u32                         layer)
{
    pimpl_->components_.push_back(comp);
    pimpl_->layout_container();
    on_preferred_size_changed();
    
    /*
    //do_add_component(comp, layout_hint, layer);
    pimpl_->dirty_ = true;

    component_connections_type component_connections;
    component_connections.first = comp;

    // Subscribe to the component's redraw event.
    component_connections.second.push_back(
        comp->on_redraw.connect(
            std::bind(
                &container::impl::subcomponent_redraw_handler
              , pimpl_
              , std::weak_ptr<component>(comp)
              , std::placeholders::_1)));

    component_connections.second.push_back(
        comp->on_cursor_position_changed.connect(
            std::bind(
                &container::impl::subcomponent_cursor_position_change_handler
              , pimpl_
              , std::weak_ptr<component>(comp)
              , std::placeholders::_1)));

    component_connections.second.push_back(
        comp->on_layout_change.connect([this]{on_layout_change();}));

    pimpl_->component_connections_.push_back(component_connections);
    on_layout_change();

    // A redraw of the container is required.
    on_redraw({ rectangle({}, get_size()) });
    */
}

// ==========================================================================
// REMOVE_COMPONENT
// ==========================================================================
void container::remove_component(std::shared_ptr<component> const &comp)
{
    pimpl_->components_.erase(std::find(
        pimpl_->components_.begin(),
        pimpl_->components_.end(),
        comp));
    pimpl_->layout_container();
    on_preferred_size_changed();
    
    /*
    pimpl_->dirty_ = true;

    // Disconnect any signals for the component.
    for (auto cur = std::begin(pimpl_->component_connections_);
         cur != std::end(pimpl_->component_connections_);
        )
    {
        if (cur->first == comp)
        {
            for (auto &cnx : cur->second)
            {
                cnx.disconnect();
            }

            cur = pimpl_->component_connections_.erase(cur);
        }
        else
        {
            ++cur;
        }
    }

    //do_remove_component(comp);

    // Now that the subcomponent has been removed, it becomes necessary
    // to re-lay the container out.
    on_layout_change();
    */
}

// ==========================================================================
// DO_SET_POSITION
// ==========================================================================
void container::do_set_position(terminalpp::point const &position)
{
    pimpl_->bounds_.origin = position;
}

// ==========================================================================
// DO_GET_POSITION
// ==========================================================================
terminalpp::point container::do_get_position() const
{
    return pimpl_->bounds_.origin;
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void container::do_set_size(terminalpp::extent const &size)
{
    pimpl_->bounds_.size = size;
} 

// ==========================================================================
// DO_GET_SIZE
// ==========================================================================
terminalpp::extent container::do_get_size() const
{
    return pimpl_->bounds_.size;
}

// ==========================================================================
// DO_GET_PREFERRED_SIZE
// ==========================================================================
terminalpp::extent container::do_get_preferred_size() const
{
    return {};
    /*
    // If there are any layouts, then find the union of their preferred
    // sizes.  Otherwise, our current size is just fine.
    if (pimpl_->layouts_.empty())
    {
        return get_size();
    }

    terminalpp::extent preferred_size(0, 0);

    // Sort the components/hints into layers
    typedef std::map<u32, std::vector<std::shared_ptr<component>>> clmap;
    typedef std::map<u32, std::vector<boost::any>>                 chmap;

    clmap component_layers_map;
    chmap component_hints_map;

    // Iterate through the components, sorting them by layer
    for (u32 index = 0; index < pimpl_->components_.size(); ++index)
    {
        auto comp =  pimpl_->components_[index];
        auto hint =  pimpl_->component_hints_[index];
        auto layer = pimpl_->component_layers_[index];

        component_layers_map[layer].push_back(comp);
        component_hints_map[layer].push_back(hint);
    }

    // Iterate through the layers, layout out each.
    for (auto &component_layer_pair : component_layers_map)
    {
        auto layer =      component_layer_pair.first;
        auto components = component_layer_pair.second;
        auto hints =      component_hints_map[layer];

        auto lyt = get_layout(layer);

        if (!lyt || components.size() == 0)
        {
            // Either there is no layout for this layer, or there are no
            // components in this layer.  Hence no point in laying it out.
            // Continue with the next layer.
            continue;
        }

        auto lp_preferred_size = lyt->get_preferred_size(components, hints);

        preferred_size.width =
            (std::max)(preferred_size.width, lp_preferred_size.width);
        preferred_size.height =
            (std::max)(preferred_size.height, lp_preferred_size.height);
    }

    return preferred_size;
    */
}

// ==========================================================================
// DO_HAS_FOCUS
// ==========================================================================
bool container::do_has_focus() const
{
    return false;
    // return pimpl_->has_focus_;
}

// ==========================================================================
// DO_SET_FOCUS
// ==========================================================================
void container::do_set_focus()
{
    /*
    if (is_enabled())
    {
        // Find the first component that can be focussed and focus it.
        for (auto const &current_component : pimpl_->components_)
        {
            if (current_component->is_enabled())
            {
                current_component->set_focus();
                break;
            }
        }

        pimpl_->has_focus_ = true;
        on_focus_set();
    }
    */
}

// ==========================================================================
// DO_LOSE_FOCUS
// ==========================================================================
void container::do_lose_focus()
{
    /*
    for (auto const &current_component : pimpl_->components_)
    {
        if (current_component->has_focus())
        {
            current_component->lose_focus();
        }
    }

    pimpl_->has_focus_ = false;
    */
}

// ==========================================================================
// DO_FOCUS_NEXT
// ==========================================================================
void container::do_focus_next()
{
    /*
    if (pimpl_->has_focus_)
    {
        pimpl_->focus_next_has_focus();
    }
    else
    {
        pimpl_->focus_next_no_focus();
    }
    */
}

// ==========================================================================
// DO_FOCUS_PREVIOUS
// ==========================================================================
void container::do_focus_previous()
{
    /*
    if (pimpl_->has_focus_)
    {
        pimpl_->focus_previous_has_focus();
    }
    else
    {
        pimpl_->focus_previous_no_focus();
    }
    */
}

// ==========================================================================
// DO_ENABLE
// ==========================================================================
void container::do_enable()
{
    // pimpl_->enabled_ = true;
}

// ==========================================================================
// DO_DISABLE
// ==========================================================================
void container::do_disable()
{
    // pimpl_->enabled_ = false;
}

// ==========================================================================
// DO_IS_ENABLED
// ==========================================================================
bool container::do_is_enabled() const
{
    return false;
    // return pimpl_->enabled_;
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool container::do_get_cursor_state() const
{
    return false;
    // return pimpl_->cursor_state_;
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point container::do_get_cursor_position() const
{
    return {};
    /*
    // If we have no focus, then return the default position.
    if (pimpl_->has_focus_ && pimpl_->cursor_state_)
    {
        // Find the subcomponent that has focus and get its cursor
        // position.  This must then be offset by the subcomponent's
        // position within our container.
        for (auto const &current_component : pimpl_->components_)
        {
            if (current_component->has_focus())
            {
                return current_component->get_position()
                     + current_component->get_cursor_position();
            }
        }
    }

    // Either we do not have focus, or the currently focussed subcomponent
    // does not have a cursor.  Return the default position.
    return {};
    */
}

// ==========================================================================
// DO_SET_CURSOR_POSITIONG
// ==========================================================================
void container::do_set_cursor_position(terminalpp::point const &position)
{
    /*
    // If we have no focus, then ignore this.
    if (pimpl_->has_focus_ && pimpl_->cursor_state_)
    {
        // Find the subcomponent that has focus and set its cursor
        // position.  This must then be offset by the subcomponent's
        // position within our container.
        for (auto const &current_component : pimpl_->components_)
        {
            if (current_component->has_focus())
            {
                current_component->set_cursor_position(
                    position - current_component->get_position());
            }
        }
    }
    */
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void container::do_layout()
{
    /*
    // Sort the components/hints into layers
    typedef std::map<u32, std::vector<std::shared_ptr<component>>> clmap;
    typedef std::map<u32, std::vector<boost::any>>                 chmap;

    clmap component_layers_map;
    chmap component_hints_map;

    // Iterate through the components, sorting them by layer
    for (u32 index = 0; index < pimpl_->components_.size(); ++index)
    {
        auto comp =  pimpl_->components_[index];
        auto hint =  pimpl_->component_hints_[index];
        auto layer = pimpl_->component_layers_[index];

        component_layers_map[layer].push_back(comp);
        component_hints_map[layer].push_back(hint);
    }

    auto size = get_size();

    // Iterate through the layers, layout out each.
    for (auto const &component_layer_pair : component_layers_map)
    {
        auto layer =      component_layer_pair.first;
        auto components = component_layer_pair.second;
        auto hints =      component_hints_map[layer];

        auto lyt = get_layout(layer);

        if (!lyt || components.size() == 0)
        {
            // Either there is no layout for this layer, or there are no
            // components in this layer.  Hence no point in laying it out.
            // Continue with the next layer.
            continue;
        }

        (*lyt)(components, hints, size);
    }

    // Now that all the sizes are correct for this container, iterate through
    // each subcomponent, and lay them out in turn.
    for (auto const &comp : pimpl_->components_)
    {
        comp->layout();
    }
    */
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void container::do_draw(context &ctx, rectangle const &region) const
{
    /*
    auto &cvs = ctx.get_canvas();

    // First, we obtain a list of components sorted by layer from lowest
    // to highest.
    pimpl_->ensure_components_sorted();

    for (auto const &current_component : pimpl_->components_)
    {
        // The area we want to draw is the intersection of the region
        // passed in above and the region of space that the component
        // occupies.
        rectangle component_region(
            current_component->get_position()
          , current_component->get_size());

        auto draw_region = intersection(region, component_region);

        if (draw_region.is_initialized())
        {
            // The draw region is currently relative to this container's
            // origin.  It should be relative to the child's origin.
            draw_region->origin -= component_region.origin;

            // The canvas must have an offset applied to it so that the
            // inner component can pretend that it is being drawn with its
            // container being at position (0,0).
            auto const position = current_component->get_position();
            cvs.offset_by({position.x, position.y});

            // Ensure that the offset is unapplied before exit of this
            // function.
            BOOST_SCOPE_EXIT( (&cvs)(&position) )
            {
                cvs.offset_by({-position.x, -position.y});
            } BOOST_SCOPE_EXIT_END

            current_component->draw(ctx, draw_region.get());
        }
    }
    */
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void container::do_event(boost::any const &event)
{
    /*
    // We split the events into two types.  Mouse events are passed to
    // whichever component is under the mouse click.  All other events are
    // passed to the focussed component.
    auto report = boost::any_cast<terminalpp::ansi::mouse::report>(&event);

    if (report != NULL)
    {
        for (auto const &current_component : pimpl_->components_)
        {
            auto position = current_component->get_position();
            auto size =     current_component->get_size();

            // Check to see if the reported position is within the component's
            // bounds.
            if (report->x_position_ >= position.x
             && report->x_position_  < position.x + size.width
             && report->y_position_ >= position.y
             && report->y_position_  < position.y + size.height)
            {
                // Copy the mouse's report and adjust it so that the
                // subcomponent's position is taken into account.
                terminalpp::ansi::mouse::report subreport;
                subreport.button_     = report->button_;
                subreport.x_position_ = u8(report->x_position_ - position.x);
                subreport.y_position_ = u8(report->y_position_ - position.y);

                // Forward the event onto the component, then look no further.
                current_component->event(subreport);
                break;
            }
        }
    }
    else
    {
        for (auto const &current_component : pimpl_->components_)
        {
            if (current_component->has_focus())
            {
                current_component->event(event);
                break;
            }
        }
    }
    */
}

// ==========================================================================
// MAKE_CONTAINER
// ==========================================================================
std::shared_ptr<container> make_container()
{
    return std::make_shared<container>();
}

}

