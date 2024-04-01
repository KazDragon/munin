#include "munin/container.hpp"

#include "munin/detail/algorithm.hpp"
#include "munin/detail/json_adaptors.hpp"
#include "munin/layout.hpp"
#include "munin/null_layout.hpp"
#include "munin/render_surface.hpp"

#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/scope_exit.hpp>
#include <terminalpp/mouse.hpp>
#include <terminalpp/rectangle.hpp>

#include <memory>
#include <vector>

namespace munin {

namespace {

using component_connections = std::vector<boost::signals2::connection>;

template <class ForwardRange>
auto find_first_focussed_component(ForwardRange const &rng)
{
    auto const &component_has_focus = [](auto const &comp) {
        return comp->has_focus();
    };

    return boost::find_if(rng, component_has_focus);
}

template <class ForwardRange, class IncrementFunction>
auto increment_focus(ForwardRange const &rng, IncrementFunction &&increment)
{
    return boost::find_if(rng, std::forward<IncrementFunction>(increment));
}

template <class ForwardRange>
auto find_component_at_point(
    ForwardRange const &rng, terminalpp::point const &location)
{
    auto const &has_location_at_point = [&location](auto const &comp) {
        auto const &position = comp->get_position();
        auto const &size = comp->get_size();

        // Check to see if the reported position is within the component's
        // bounds.
        return (
            location.x_ >= position.x_
            && location.x_ < position.x_ + size.width_
            && location.y_ >= position.y_
            && location.y_ < position.y_ + size.height_);
    };

    return boost::find_if(rng, has_location_at_point);
}

}  // namespace

// ==========================================================================
// CONTAINER::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct container::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    explicit impl(container &self) : self_(self)
    {
    }

    // ======================================================================
    // SET_LAYOUT
    // ======================================================================
    void set_layout(std::unique_ptr<munin::layout> &&lyt)
    {
        layout_ = lyt == nullptr ? make_null_layout() : std::move(lyt);
        layout_container();
    }

    // ======================================================================
    // ADD_COMPONENT
    // ======================================================================
    void add_component(
        std::shared_ptr<component> const &comp, boost::any const &layout_hint)
    {
        component_connections cnx;

        cnx.push_back(comp->on_focus_set.connect(
            [this, wcomp = std::weak_ptr<component>(comp)] {
                this->subcomponent_focus_set_handler(wcomp);
            }));

        cnx.push_back(comp->on_focus_lost.connect(
            [this] { this->subcomponent_focus_lost_handler(); }));

        cnx.push_back(comp->on_cursor_state_changed.connect(
            [this, wcomp = std::weak_ptr<component>(comp)] {
                this->subcomponent_cursor_state_change_handler(wcomp);
            }));

        cnx.push_back(comp->on_cursor_position_changed.connect(
            [this, wcomp = std::weak_ptr<component>(comp)] {
                this->subcomponent_cursor_position_change_handler(wcomp);
            }));

        cnx.push_back(comp->on_redraw.connect(
            [this, wcomp = std::weak_ptr<component>(comp)](
                auto const &redraw_regions) {
                this->subcomponent_redraw_handler(wcomp, redraw_regions);
            }));

        components_.push_back(comp);
        hints_.push_back(layout_hint);
        component_connections_.push_back(cnx);
        layout_container();
        self_.on_preferred_size_changed();
    }

    // ======================================================================
    // REMOVE_COMPONENT
    // ======================================================================
    void remove_component(std::shared_ptr<component> const &comp)
    {
        auto const &disconnect_connection = [](auto &cnx) { cnx.disconnect(); };

        for (auto index = 0; index < components_.size(); ++index)
        {
            if (components_[index] == comp)
            {
                components_.erase(components_.begin() + index);
                hints_.erase(hints_.begin() + index);
                boost::for_each(
                    component_connections_[index], disconnect_connection);

                component_connections_.erase(
                    component_connections_.begin() + index);
            }
        }

        layout_container();
        self_.on_preferred_size_changed();
    }

    // ======================================================================
    // SET_POSITION
    // ======================================================================
    void set_position(terminalpp::point const &position)
    {
        bounds_.origin_ = position;
    }

    // ======================================================================
    // GET_POSITION
    // ======================================================================
    [[nodiscard]] terminalpp::point get_position() const
    {
        return bounds_.origin_;
    }

    // ======================================================================
    // SET_SIZE
    // ======================================================================
    void set_size(terminalpp::extent const &size)
    {
        bounds_.size_ = size;
        layout_container();
    }

    // ======================================================================
    // GET_SIZE
    // ======================================================================
    [[nodiscard]] terminalpp::extent get_size() const
    {
        return bounds_.size_;
    }

    // ======================================================================
    // GET_PREFERRED_SIZE
    // ======================================================================
    [[nodiscard]] terminalpp::extent get_preferred_size() const
    {
        return layout_->get_preferred_size(components_, hints_);
    }

    // ==========================================================================
    // DO_HAS_FOCUS
    // ==========================================================================
    [[nodiscard]] bool has_focus() const
    {
        return has_focus_;
    }

    // ======================================================================
    // SET_FOCUS
    // ======================================================================
    void set_focus()
    {
        in_focus_operation_ = true;

        BOOST_SCOPE_EXIT_ALL(this)
        {
            in_focus_operation_ = false;
        };

        if (!has_focus_)
        {
            auto const &set_component_focus = [](auto const &comp) {
                comp->set_focus();
                return comp->has_focus();
            };

            auto const &focussed_component =
                increment_focus(components_, set_component_focus);

            has_focus_ = focussed_component != components_.end();

            if (has_focus_)
            {
                self_.on_focus_set();
                self_.on_cursor_state_changed();
                self_.on_cursor_position_changed();
            }
        }
    }

    // ======================================================================
    // LOSE_FOCUS
    // ======================================================================
    void lose_focus()
    {
        in_focus_operation_ = true;

        BOOST_SCOPE_EXIT_ALL(this)
        {
            in_focus_operation_ = false;
        };

        if (auto focussed_component =
                find_first_focussed_component(components_);
            focussed_component != components_.end())
        {
            (*focussed_component)->lose_focus();
            has_focus_ = false;
            self_.on_focus_lost();
            self_.on_cursor_state_changed();
            self_.on_cursor_position_changed();
        }
    }

    // ======================================================================
    // FOCUS_NEXT
    // ======================================================================
    void focus_next()
    {
        auto const &focus_next_component = [](auto const &comp) {
            comp->focus_next();
            return comp->has_focus();
        };

        focus_incremental(components_, focus_next_component);
    }

    // ======================================================================
    // FOCUS_PREVIOUS
    // ======================================================================
    void focus_previous()
    {
        using boost::adaptors::reversed;

        auto const &focus_previous_component = [](auto const &comp) {
            comp->focus_previous();
            return comp->has_focus();
        };

        focus_incremental(components_ | reversed, focus_previous_component);
    }

    // ======================================================================
    // GET_CURSOR_STATE
    // ======================================================================
    [[nodiscard]] bool get_cursor_state() const
    {
        auto comp = find_first_focussed_component(components_);

        return comp == components_.end() ? false : (*comp)->get_cursor_state();
    }

    // ======================================================================
    // GET_CURSOR_POSITION
    // ======================================================================
    [[nodiscard]] terminalpp::point get_cursor_position() const
    {
        auto comp = find_first_focussed_component(components_);

        return comp == components_.end()
                 ? terminalpp::point{}
                 : (*comp)->get_position() + (*comp)->get_cursor_position();
    }

    // ======================================================================
    // SET_CURSOR_POSITION
    // ======================================================================
    void set_cursor_position(terminalpp::point const &position)
    {
        // Note: Setting the cursor position on a container doesn't really
        // make too much sense, but an implementation is required to fulfil the
        // component interface.  Our default implementation sets the relative
        // cursor position in the focussed component.
        auto comp = find_first_focussed_component(components_);

        if (comp != components_.end())
        {
            (*comp)->set_cursor_position(position - (*comp)->get_position());
        }
    }

    // ======================================================================
    // DRAW
    // ======================================================================
    void draw(
        render_surface &surface, terminalpp::rectangle const &region) const
    {
        for (auto const &comp : components_)
        {
            draw_component(comp, surface, region);
        }
    }

    // ======================================================================
    // EVENT
    // ======================================================================
    void event(boost::any const &ev)
    {
        // We split incoming events into two types:
        // * Common events (e.g. keypressed, etc.) are passed on to the
        //   subcomponent with focus.
        // * Mouse events are passed on to the subcomponent at the location
        //   of the event, and the co-ordinates of the event are passed on
        //   relative to the subcomponent's location.
        if (auto const *mouse_event =
                boost::any_cast<terminalpp::mouse::event>(&ev);
            mouse_event == nullptr)
        {
            handle_common_event(ev);
        }
        else
        {
            handle_mouse_event(*mouse_event);
        }
    }

    // ======================================================================
    // TO_JSON
    // ======================================================================
    [[nodiscard]] nlohmann::json to_json() const
    {
        nlohmann::json json = {
            {"type",            "container"                           },
            {"position",        detail::to_json(get_position())       },
            {"size",            detail::to_json(get_size())           },
            {"preferred_size",  detail::to_json(get_preferred_size()) },
            {"has_focus",       has_focus()                           },
            {"cursor_state",    get_cursor_state()                    },
            {"cursor_position", detail::to_json(get_cursor_position())},
            {"layout",          layout_->to_json()                    }
        };

        auto &subcomponents = json["subcomponents"];

        for (auto index = size_t{0}; index < components_.size(); ++index)
        {
            subcomponents[index] = components_[index]->to_json();
        }

        return json;
    }

private:
    // ======================================================================
    // LAYOUT_CONTAINER
    // ======================================================================
    void layout_container()
    {
        (*layout_)(components_, hints_, bounds_.size_);
    }

    // ======================================================================
    // FOCUS_INCREMENTAL
    // ======================================================================
    template <typename ForwardRange, typename Op>
    void focus_incremental(ForwardRange const &components, Op &&increment_op)
    {
        using std::cbegin;
        using std::cend;

        in_focus_operation_ = true;

        BOOST_SCOPE_EXIT_ALL(this)
        {
            in_focus_operation_ = false;
        };

        bool const had_focus = has_focus_;

        auto const &first_focussed_component =
            find_first_focussed_component(components);

        auto const &increment_from =
            first_focussed_component == cend(components)
                ? cbegin(components)
                : first_focussed_component;

        auto const &incrementally_focussed_component = increment_focus(
            boost::make_iterator_range(increment_from, cend(components)),
            std::forward<Op>(increment_op));

        has_focus_ = incrementally_focussed_component != cend(components);

        // Announce a change in focus if that changed.
        if (had_focus != has_focus_)
        {
            if (has_focus_)
            {
                self_.on_focus_set();
            }
            else
            {
                self_.on_focus_lost();
            }

            self_.on_cursor_position_changed();
            self_.on_cursor_state_changed();
        }

        // If we had focus continuously, but the focussed subcomponent changed,
        // then we also want to announce cursor changes, since even though the
        // position and state of the cursor in the individual subcomponents
        // hasn't changed (and therefore they have no reason to send such a
        // signal), we know that the cursor may have moved about due to focus.
        if (had_focus && has_focus_
            && increment_from != incrementally_focussed_component)
        {
            self_.on_cursor_position_changed();
            self_.on_cursor_state_changed();
        }
    }

    // ======================================================================
    // DRAW_COMPONENT
    // ======================================================================
    void draw_component(
        std::shared_ptr<component> const &comp,
        render_surface &surface,
        terminalpp::rectangle const &region) const
    {
        auto const component_region =
            terminalpp::rectangle{comp->get_position(), comp->get_size()};

        if (auto draw_region = detail::intersection(component_region, region);
            draw_region)
        {
            // The draw region is currently relative to this container's
            // origin.  It should be relative to the child's origin.
            draw_region->origin_ -= component_region.origin_;

            // The canvas must have an offset applied to it so that the
            // inner component can pretend that it is being drawn with its
            // container being at position (0,0).
            surface.offset_by(
                {component_region.origin_.x_, component_region.origin_.y_});

            // Ensure that the offset is unapplied before exit of this
            // function.
            BOOST_SCOPE_EXIT_ALL(&surface, &component_region)
            {
                surface.offset_by(
                    {-component_region.origin_.x_,
                     -component_region.origin_.y_});
            };

            comp->draw(surface, draw_region.value());
        }
    }

    // ======================================================================
    // SUBCOMPONENT_REDRAW_HANDLER
    // ======================================================================
    void subcomponent_redraw_handler(
        std::weak_ptr<component> const &weak_subcomponent,
        std::vector<terminalpp::rectangle> regions)
    {
        auto subcomponent = weak_subcomponent.lock();

        if (subcomponent != nullptr)
        {
            // Each region is bound to the origin of the component in question.
            // It must be rebound to the origin of the container.  We do this
            // by offsetting the regions' origins by the origin of the
            // subcomponent within this container.
            auto origin = subcomponent->get_position();

            for (auto &rect : regions)
            {
                rect.origin_.x_ += origin.x_;
                rect.origin_.y_ += origin.y_;
            }

            // This new information must be passed up the component heirarchy.
            self_.on_redraw(regions);
        }
    }

    // ======================================================================
    // SUBCOMPONENT_FOCUS_SET_HANDLER
    // ======================================================================
    void subcomponent_focus_set_handler(
        std::weak_ptr<component> const &weak_comp)
    {
        if (!in_focus_operation_)
        {
            auto const &another_component_has_focus =
                [orig = weak_comp.lock()](auto const &comp) {
                    return comp != orig && comp->has_focus();
                };

            if (auto comp =
                    boost::find_if(components_, another_component_has_focus);
                comp != components_.end())
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

            self_.on_cursor_position_changed();
            self_.on_cursor_state_changed();
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
        std::weak_ptr<component> const &weak_subcomponent)
    {
        if (auto subcomponent = weak_subcomponent.lock();
            subcomponent && subcomponent->has_focus())
        {
            self_.on_cursor_state_changed();
        }
    }

    // ======================================================================
    // SUBCOMPONENT_CURSOR_POSITION_CHANGE_HANDLER
    // ======================================================================
    void subcomponent_cursor_position_change_handler(
        std::weak_ptr<component> const &weak_subcomponent)
    {
        if (auto subcomponent = weak_subcomponent.lock();
            subcomponent && subcomponent->has_focus())
        {
            self_.on_cursor_position_changed();
        }
    }

    // ======================================================================
    // HANDLE_COMMON_EVENT
    // ======================================================================
    void handle_common_event(boost::any const &event)
    {
        if (auto comp = find_first_focussed_component(components_);
            comp != components_.end())
        {
            (*comp)->event(event);
        }
    }

    // ======================================================================
    // HANDLE_MOUSE_EVENT
    // ======================================================================
    void handle_mouse_event(terminalpp::mouse::event const &ev)
    {
        if (auto const &comp =
                find_component_at_point(components_, ev.position_);
            comp != components_.end())
        {
            auto const &position = (*comp)->get_position();

            (*comp)->event(
                terminalpp::mouse::event{ev.action_, ev.position_ - position});
        }
    }

    container &self_;
    terminalpp::rectangle bounds_;
    std::unique_ptr<munin::layout> layout_ = make_null_layout();
    std::vector<std::shared_ptr<component>> components_;
    std::vector<boost::any> hints_;
    std::vector<component_connections> component_connections_;
    bool has_focus_ = false;
    bool in_focus_operation_ = false;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
container::container() : pimpl_(std::make_unique<impl>(*this))
{
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
container::~container() = default;

// ==========================================================================
// SET_LAYOUT
// ==========================================================================
void container::set_layout(std::unique_ptr<munin::layout> &&lyt)
{
    pimpl_->set_layout(std::move(lyt));
}

// ==========================================================================
// ADD_COMPONENT
// ==========================================================================
void container::add_component(
    std::shared_ptr<component> const &comp, boost::any const &layout_hint)
{
    pimpl_->add_component(comp, layout_hint);
}

// ==========================================================================
// REMOVE_COMPONENT
// ==========================================================================
void container::remove_component(std::shared_ptr<component> const &comp)
{
    pimpl_->remove_component(comp);
}

// ==========================================================================
// DO_SET_POSITION
// ==========================================================================
void container::do_set_position(terminalpp::point const &position)
{
    pimpl_->set_position(position);
}

// ==========================================================================
// DO_GET_POSITION
// ==========================================================================
terminalpp::point container::do_get_position() const
{
    return pimpl_->get_position();
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void container::do_set_size(terminalpp::extent const &size)
{
    pimpl_->set_size(size);
}

// ==========================================================================
// DO_GET_SIZE
// ==========================================================================
terminalpp::extent container::do_get_size() const
{
    return pimpl_->get_size();
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
    return pimpl_->has_focus();
}

// ==========================================================================
// DO_SET_FOCUS
// ==========================================================================
void container::do_set_focus()
{
    pimpl_->set_focus();
}

// ==========================================================================
// DO_LOSE_FOCUS
// ==========================================================================
void container::do_lose_focus()
{
    pimpl_->lose_focus();
}

// ==========================================================================
// DO_FOCUS_NEXT
// ==========================================================================
void container::do_focus_next()
{
    pimpl_->focus_next();
}

// ==========================================================================
// DO_FOCUS_PREVIOUS
// ==========================================================================
void container::do_focus_previous()
{
    pimpl_->focus_previous();
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool container::do_get_cursor_state() const
{
    return pimpl_->get_cursor_state();
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point container::do_get_cursor_position() const
{
    return pimpl_->get_cursor_position();
}

// ==========================================================================
// DO_SET_CURSOR_POSITION
// ==========================================================================
void container::do_set_cursor_position(terminalpp::point const &position)
{
    pimpl_->set_cursor_position(position);
}

// ==========================================================================
// DO_DRAW
// ==========================================================================
void container::do_draw(
    render_surface &surface, terminalpp::rectangle const &region) const
{
    pimpl_->draw(surface, region);
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void container::do_event(boost::any const &event)
{
    pimpl_->event(event);
}

// ==========================================================================
// DO_TO_JSON
// ==========================================================================
nlohmann::json container::do_to_json() const
{
    return pimpl_->to_json();
}

// ==========================================================================
// MAKE_CONTAINER
// ==========================================================================
std::shared_ptr<container> make_container()
{
    return std::make_shared<container>();
}

}  // namespace munin
