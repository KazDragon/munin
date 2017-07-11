#include "munin/container.hpp"
#include "munin/algorithm.hpp"
#include "munin/context.hpp"
#include "munin/layout.hpp"
#include "munin/null_layout.hpp"
#include "munin/rectangle.hpp"
#include <terminalpp/ansi/mouse.hpp>
#include <terminalpp/canvas_view.hpp>
#include <boost/optional.hpp>
#include <boost/scope_exit.hpp>
#include <vector>

namespace munin {

/*
namespace {
    using component_connections_type =
        std::pair<
            std::shared_ptr<component>,
            std::vector<boost::signals2::connection>>;
}
*/

namespace {

template <class ForwardIterator>
static ForwardIterator find_first_focussed_component(
    ForwardIterator begin,
    ForwardIterator end)
{
    return std::find_if(
        begin,
        end,
        [](auto const &comp)
        {
            return comp->has_focus();
        });
}

template <class ForwardIterator, class IncrementFunction>
static boost::optional<bool> increment_focus(
    bool has_focus,
    ForwardIterator begin,
    ForwardIterator end,
    IncrementFunction &&increment)
{
    if (has_focus)
    {
        begin = find_first_focussed_component(begin, end);
        assert(begin != end);
    }

    if ((std::find_if(
             begin,
             end,
             std::forward<IncrementFunction>(increment))
         == end) == has_focus)
    {
        return !has_focus;
    }
    else
    {
        return {};
    }
}

template <class ForwardIterator>
static ForwardIterator find_component_at_point(
    ForwardIterator begin,
    ForwardIterator end,
    terminalpp::point const &location)
{
    return std::find_if(
        begin,
        end,
        [&location](auto const &comp)
        {
            auto const &position = comp->get_position();
            auto const &size     = comp->get_size();

            // Check to see if the reported position is within the component's
            // bounds.
            return (location.x >= position.x
                 && location.x  < position.x + size.width
                 && location.y >= position.y
                 && location.y < position.y + size.height);
        });
}

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
      : self_(self)
    {
    }

    // ======================================================================
    // LAYOUT_CONTAINER
    // ======================================================================
    void layout_container()
    {
        (*layout_)(components_, hints_, bounds_.size);
    }

    // ======================================================================
    // GET_PREFERRED_SIZE
    // ======================================================================
    terminalpp::extent get_preferred_size() const
    {
        return layout_->get_preferred_size(components_, {});
    }

    // ======================================================================
    // DRAW_COMPONENTS
    // ======================================================================
    void draw_components(context &ctx, rectangle const &region) const
    {
        for (auto const &comp : components_)
        {
            draw_component(comp, ctx, region);
        }
    }

    // ======================================================================
    // DRAW_COMPONENT
    // ======================================================================
    void draw_component(
        std::shared_ptr<component> const &comp,
        context &ctx,
        rectangle const &region) const
    {
        auto const component_region = rectangle {
            comp->get_position(),
            comp->get_size()
        };

        auto draw_region = intersection(component_region, region);

        if (draw_region)
        {
            // The draw region is currently relative to this container's
            // origin.  It should be relative to the child's origin.
            draw_region->origin -= component_region.origin;

            // The canvas must have an offset applied to it so that the
            // inner component can pretend that it is being drawn with its
            // container being at position (0,0).
            auto &cvs = ctx.get_canvas();

            cvs.offset_by({
                component_region.origin.x,
                component_region.origin.y
            });

            // Ensure that the offset is unapplied before exit of this
            // function.
            BOOST_SCOPE_EXIT_ALL(&cvs, &component_region)
            {
                cvs.offset_by({
                    -component_region.origin.x,
                    -component_region.origin.y
                });
            };

            comp->draw(ctx, draw_region.get());
        }
    }

    // ======================================================================
    // SUBCOMPONENT_REDRAW_HANDLER
    // ======================================================================
    /*
    void subcomponent_redraw_handler(
        std::weak_ptr<component> weak_subcomponent,
        std::vector<rectangle>   regions)
    {
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
    }
    */

    // ======================================================================
    // SUBCOMPONENT_FOCUS_SET_HANDLER
    // ======================================================================
    void subcomponent_focus_set_handler(
        std::weak_ptr<component> const &weak_comp)
    {
        if (!in_focus_operation_)
        {
            auto comp = std::find_if(
                components_.begin(),
                components_.end(),
                [orig = weak_comp.lock()](auto const &comp)
            {
                return comp != orig && comp->has_focus();
            });

            if (comp != components_.end())
            {
                in_focus_operation_ = true;

                BOOST_SCOPE_EXIT_ALL(this)
                {
                    in_focus_operation_ = false;
                };

                (*comp)->lose_focus();
            }
            else
            {
                has_focus_ = true;
                self_.on_focus_set();
            }
        }
    }

    // ======================================================================
    // SUBCOMPONENT_FOCUS_LOST_HANDLER
    // ======================================================================
    void subcomponent_focus_lost_handler()
    {
        if (!in_focus_operation_)
        {
            has_focus_ = false;
            self_.on_focus_lost();
        }
    }

    // ======================================================================
    // SUBCOMPONENT_CURSOR_STATE_CHANGE_HANDLER
    // ======================================================================
    void subcomponent_cursor_state_change_handler(
        std::weak_ptr<component> weak_subcomponent)
    {
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent && subcomponent->has_focus())
        {
            self_.on_cursor_state_changed();
        }
    }

    // ======================================================================
    // SUBCOMPONENT_CURSOR_POSITION_CHANGE_HANDLER
    // ======================================================================
    void subcomponent_cursor_position_change_handler(
        std::weak_ptr<component> weak_subcomponent)
    {
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent && subcomponent->has_focus())
        {
            self_.on_cursor_position_changed();
        }
    }

    // ======================================================================
    // DO_EVENT
    // ======================================================================
    void do_event(boost::any const &event)
    {
        // We split incoming events into two types:
        // * Common events (e.g. keypressed, etc.) are passed on to the
        //   subcomponent with focus.
        // * Mouse events are passed on to the subcomponent at the location
        //   of the event, and the co-ordinates of the event are passed on
        //   relative to the subcomponent's location.
        auto const *report = boost::any_cast<terminalpp::ansi::mouse::report>(&event);

        if (report == nullptr)
        {
            handle_common_event(event);
        }
        else
        {
            handle_mouse_event(*report);
        }
    }

    // ======================================================================
    // HANDLE_COMMON_EVENT
    // ======================================================================
    void handle_common_event(boost::any const &event)
    {
        auto comp = find_first_focussed_component(
            components_.begin(), components_.end());

        if (comp != components_.end())
        {
            (*comp)->event(event);
        }
    }

    // ======================================================================
    // HANDLE_MOUSE_EVENT
    // ======================================================================
    void handle_mouse_event(terminalpp::ansi::mouse::report const &report)
    {
        auto const &comp = find_component_at_point(
            components_.begin(),
            components_.end(),
            terminalpp::point(report.x_position_, report.y_position_));

        if (comp != components_.end())
        {
            auto const &position = (*comp)->get_position();

            (*comp)->event(
                terminalpp::ansi::mouse::report {
                    report.button_,
                    report.x_position_ - position.x,
                    report.y_position_ - position.y
                });
        }
    }

    container                               &self_;
    munin::rectangle                         bounds_;
    std::unique_ptr<munin::layout>           layout_ = make_null_layout();
    std::vector<std::shared_ptr<component>>  components_;
    std::vector<boost::any>                  hints_;
    bool                                     enabled_ = true;
    bool                                     has_focus_ = false;
    bool                                     in_focus_operation_ = false;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
container::container()
{
    pimpl_ = std::make_shared<impl>(std::ref(*this));
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
void container::set_layout(std::unique_ptr<munin::layout> &&lyt)
{
    pimpl_->layout_ = std::move(lyt);
    pimpl_->layout_container();
}

// ==========================================================================
// ADD_COMPONENT
// ==========================================================================
void container::add_component(
    std::shared_ptr<component> const &comp
  , boost::any                 const &layout_hint)
{
    comp->on_focus_set.connect(
        [this, wcomp = std::weak_ptr<component>(comp)]
        {
            pimpl_->subcomponent_focus_set_handler(wcomp);
        });

    comp->on_focus_lost.connect(
        [this]
        {
            pimpl_->subcomponent_focus_lost_handler();
        });

    comp->on_cursor_state_changed.connect(
        [this, wcomp = std::weak_ptr<component>(comp)]
        {
            pimpl_->subcomponent_cursor_state_change_handler(wcomp);
        });

    comp->on_cursor_position_changed.connect(
        [this, wcomp = std::weak_ptr<component>(comp)]
        {
            pimpl_->subcomponent_cursor_position_change_handler(wcomp);
        });

    pimpl_->components_.push_back(comp);
    pimpl_->hints_.push_back(layout_hint);
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
    for (auto index = 0; index < pimpl_->components_.size(); ++index)
    {
        if (pimpl_->components_[index] == comp)
        {
            pimpl_->components_.erase(pimpl_->components_.begin() + index);
            pimpl_->hints_.erase(pimpl_->hints_.begin() + index);
            
        }
    }
    
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
    pimpl_->layout_container();
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
    return pimpl_->get_preferred_size();
}

// ==========================================================================
// DO_HAS_FOCUS
// ==========================================================================
bool container::do_has_focus() const
{
    return pimpl_->has_focus_;
}

// ==========================================================================
// DO_SET_FOCUS
// ==========================================================================
void container::do_set_focus()
{
    pimpl_->in_focus_operation_ = true;

    BOOST_SCOPE_EXIT_ALL(this)
    {
        pimpl_->in_focus_operation_ = false;
    };

    if (!pimpl_->has_focus_)
    {
        if (increment_focus(
            false,
            pimpl_->components_.begin(),
            pimpl_->components_.end(),
            [](auto const &comp)
            {
                comp->set_focus();
                return comp->has_focus();
            }))
        {
            pimpl_->has_focus_ = true;
            on_focus_set();
            on_cursor_state_changed();
            on_cursor_position_changed();
        }

    }
}

// ==========================================================================
// DO_LOSE_FOCUS
// ==========================================================================
void container::do_lose_focus()
{
    pimpl_->in_focus_operation_ = true;

    BOOST_SCOPE_EXIT_ALL(this)
    {
        pimpl_->in_focus_operation_ = false;
    };

    auto focussed_component =
        std::find_if(pimpl_->components_.begin(), pimpl_->components_.end(),
            [](auto const &component)
            {
                return component->has_focus();
            });

    if (focussed_component != pimpl_->components_.end())
    {
        (*focussed_component)->lose_focus();
        pimpl_->has_focus_ = false;
        on_focus_lost();
        on_cursor_state_changed();
        on_cursor_position_changed();
    }
}

// ==========================================================================
// DO_FOCUS_NEXT
// ==========================================================================
void container::do_focus_next()
{
    pimpl_->in_focus_operation_ = true;

    BOOST_SCOPE_EXIT_ALL(this)
    {
        pimpl_->in_focus_operation_ = false;
    };

    auto focus_change = increment_focus(
        pimpl_->has_focus_,
        pimpl_->components_.begin(),
        pimpl_->components_.end(),
        [](auto const &comp)
        {
            comp->focus_next();
            return comp->has_focus();
        });

    if (focus_change)
    {
        if (*focus_change)
        {
            pimpl_->has_focus_ = true;
            on_focus_set();
        }
        else
        {
            pimpl_->has_focus_ = false;
            on_focus_lost();
        }

        on_cursor_position_changed();
        on_cursor_state_changed();
    }
}

// ==========================================================================
// DO_FOCUS_PREVIOUS
// ==========================================================================
void container::do_focus_previous()
{
    pimpl_->in_focus_operation_ = true;

    BOOST_SCOPE_EXIT_ALL(this)
    {
        pimpl_->in_focus_operation_ = false;
    };

    auto focus_change = increment_focus(
        pimpl_->has_focus_,
        pimpl_->components_.rbegin(),
        pimpl_->components_.rend(),
        [](auto const &comp)
        {
            comp->focus_previous();
            return comp->has_focus();
        });

    if (focus_change)
    {
        pimpl_->has_focus_ = *focus_change;

        if (pimpl_->has_focus_)
        {
            on_focus_set();
        }
        else
        {
            on_focus_lost();
        }

        on_cursor_position_changed();
        on_cursor_state_changed();
    }
}

// ==========================================================================
// DO_ENABLE
// ==========================================================================
void container::do_enable()
{
    pimpl_->enabled_ = true;
}

// ==========================================================================
// DO_DISABLE
// ==========================================================================
void container::do_disable()
{
    pimpl_->enabled_ = false;
}

// ==========================================================================
// DO_IS_ENABLED
// ==========================================================================
bool container::do_is_enabled() const
{
    return pimpl_->enabled_;
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool container::do_get_cursor_state() const
{
    auto comp = find_first_focussed_component(
        pimpl_->components_.begin(),
        pimpl_->components_.end());

    return comp == pimpl_->components_.end()
         ? false
         : (*comp)->get_cursor_state();
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point container::do_get_cursor_position() const
{
    auto comp = find_first_focussed_component(
        pimpl_->components_.begin(),
        pimpl_->components_.end());

    return comp == pimpl_->components_.end()
         ? terminalpp::point{}
         : (*comp)->get_position() + (*comp)->get_cursor_position();
}

// ==========================================================================
// DO_SET_CURSOR_POSITION
// ==========================================================================
void container::do_set_cursor_position(terminalpp::point const &position)
{
    // Note: Setting the cursor position on a container doesn't really
    // make too much sense, but an implementation is required to fulfil the
    // component interface.  Our default implementation sets the relative
    // cursor position in the focussed component.
    auto comp = find_first_focussed_component(
        pimpl_->components_.begin(),
        pimpl_->components_.end());

    if (comp != pimpl_->components_.end())
    {
        (*comp)->set_cursor_position(position - (*comp)->get_position());
    }
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void container::do_draw(context &ctx, rectangle const &region) const
{
    pimpl_->draw_components(ctx, region);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void container::do_event(boost::any const &event)
{
    pimpl_->do_event(event);
}

// ==========================================================================
// MAKE_CONTAINER
// ==========================================================================
std::shared_ptr<container> make_container()
{
    return std::make_shared<container>();
}

}

