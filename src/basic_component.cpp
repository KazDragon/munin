#include "munin/basic_component.hpp"
#include <terminalpp/ansi/mouse.hpp>
#include <map>

namespace munin {

// ==========================================================================
// BASIC_COMPONENT::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct basic_component::impl
{
    // ======================================================================
    // CONSTRUCTOR
    // ======================================================================
    impl(basic_component &self)
        : self_(self)
        , has_focus_(false)
        , enabled_(true)
    {
    }

    // ======================================================================
    // TOGGLE_FOCUS
    // ======================================================================
    void toggle_focus()
    {
        if (has_focus_)
        {
            has_focus_ = false;
            self_.on_focus_lost();
        }
        else
        {
            has_focus_ = true;
            self_.on_focus_set();
        }
    }

    basic_component                  &self_;
    std::map<std::string, boost::any> attributes_;
    rectangle                         bounds_;
    bool                              has_focus_;
    bool                              enabled_;
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
basic_component::basic_component()
{
    pimpl_ = std::make_shared<impl>(std::ref(*this));
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
basic_component::~basic_component()
{
}

// ==========================================================================
// DO_SET_POSITION
// ==========================================================================
void basic_component::do_set_position(terminalpp::point const &position)
{
    auto old_position = pimpl_->bounds_.origin;
    pimpl_->bounds_.origin = position;

    on_position_changed(old_position, position);
}

// ==========================================================================
// DO_GET_POSITION
// ==========================================================================
terminalpp::point basic_component::do_get_position() const
{
    return pimpl_->bounds_.origin;
}

// ==========================================================================
// DO_SET_SIZE
// ==========================================================================
void basic_component::do_set_size(terminalpp::extent const &size)
{
    if (size != pimpl_->bounds_.size)
    {
        pimpl_->bounds_.size = size;
        on_size_changed();
    }
}

// ==========================================================================
// DO_GET_SIZE
// ==========================================================================
terminalpp::extent basic_component::do_get_size() const
{
    return pimpl_->bounds_.size;
}

// ==========================================================================
// DO_HAS_FOCUS
// ==========================================================================
bool basic_component::do_has_focus() const
{
    return pimpl_->has_focus_;
}

// ==========================================================================
// DO_SET_FOCUS
// ==========================================================================
void basic_component::do_set_focus()
{
    if (pimpl_->enabled_)
    {
        bool old_focus = pimpl_->has_focus_;
        pimpl_->has_focus_ = true;
        
        if (!old_focus)
        {
            on_focus_set();
        }
    }
}

// ==========================================================================
// DO_LOSE_FOCUS
// ==========================================================================
void basic_component::do_lose_focus()
{
    bool old_focus = pimpl_->has_focus_;
    pimpl_->has_focus_ = false;
    
    if (old_focus)
    {
        on_focus_lost();
    }
}

// ==========================================================================
// DO_FOCUS_NEXT
// ==========================================================================
void basic_component::do_focus_next()
{
    if (pimpl_->enabled_)
    {
        pimpl_->toggle_focus();
    }
}

// ==========================================================================
// DO_FOCUS_PREVIOUS
// ==========================================================================
void basic_component::do_focus_previous()
{
    if (pimpl_->enabled_)
    {
        pimpl_->toggle_focus();
    }
}

// ==========================================================================
// DO_ENABLE
// ==========================================================================
void basic_component::do_enable()
{
    pimpl_->enabled_ = true;
}

// ==========================================================================
// DO_DISABLE
// ==========================================================================
void basic_component::do_disable()
{
    pimpl_->enabled_ = false;
}

// ==========================================================================
// DO_IS_ENABLED
// ==========================================================================
bool basic_component::do_is_enabled() const
{
    return pimpl_->enabled_;
}

// ==========================================================================
// DO_GET_CURSOR_STATE
// ==========================================================================
bool basic_component::do_get_cursor_state() const
{
    // By default, a component has no cursor.
    return false;
}

// ==========================================================================
// DO_GET_CURSOR_POSITION
// ==========================================================================
terminalpp::point basic_component::do_get_cursor_position() const
{
    // By default, a component has no cursor, so we choose a sentry
    // value of (0,0) for its non-existent location.
    return {};
}

// ==========================================================================
// DO_SET_CURSOR_POSITION
// ==========================================================================
void basic_component::do_set_cursor_position(terminalpp::point const &position)
{
}

// ==========================================================================
// GET_ATTRIBUTE
// ==========================================================================
boost::any basic_component::get_attribute(std::string const &name) const
{
    auto attr_iterator = pimpl_->attributes_.find(name);

    return attr_iterator == pimpl_->attributes_.end()
         ? boost::any()
         : *attr_iterator;
}

// ==========================================================================
// DO_SET_ATTRIBUTE
// ==========================================================================
void basic_component::do_set_attribute(
    std::string const &name,
    boost::any const  &attr)
{
    pimpl_->attributes_[name] = attr;
}

// ==========================================================================
// DO_LAYOUT
// ==========================================================================
void basic_component::do_layout()
{
    // By default, components are single entities and don't need laying out.
}

// ==========================================================================
// DO_EVENT
// ==========================================================================
void basic_component::do_event(boost::any const &event)
{
    auto const *mouse =
        boost::any_cast<terminalpp::ansi::mouse::report>(&event);

    if (mouse
     && mouse->button_ != terminalpp::ansi::mouse::report::BUTTON_UP)
    {
        if (!has_focus())
        {
            set_focus();
        }
    }
}

}

