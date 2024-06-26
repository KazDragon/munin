#include "munin/frame.hpp"

namespace munin {

namespace {

constexpr terminalpp::attribute default_lowlight_attribute = {};

constexpr terminalpp::attribute default_highlight_attribute = {
    terminalpp::graphics::colour::cyan,
    {},
    terminalpp::graphics::intensity::bold};

}  // namespace

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
frame::frame()
  : lowlight_attribute_{default_lowlight_attribute},
    highlight_attribute_{default_highlight_attribute}
{
}

// ==========================================================================
// HIGHLIGHT_ON_FOCUS
// ==========================================================================
void frame::highlight_on_focus(
    std::shared_ptr<component> const &inner_component)
{
    do_highlight_on_focus(inner_component);
}

// ==========================================================================
// SET_LOWLIGHT_ATTRIBUTE
// ==========================================================================
void frame::set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
    do_set_lowlight_attribute(lowlight_attribute);
}

// ==========================================================================
// SET_HIGHLIGHT_ATTRIBUTE
// ==========================================================================
void frame::set_highlight_attribute(
    terminalpp::attribute const &highlight_attribute)
{
    do_set_highlight_attribute(highlight_attribute);
}

// ==========================================================================
// DO_HIGHLIGHT_ON_FOCUS
// ==========================================================================
void frame::do_highlight_on_focus(
    std::shared_ptr<component> const &inner_component)
{
    inner_component_ = inner_component;

    auto evaluate_focus = [this]() {
        std::shared_ptr<component> comp(inner_component_);

        if (comp)
        {
            do_inner_focus_changed();
        }
    };

    inner_component->on_focus_set.connect(evaluate_focus);
    inner_component->on_focus_lost.connect(evaluate_focus);
    evaluate_focus();
}

// ==========================================================================
// DO_SET_LOWLIGHT_ATTRIBUTE
// ==========================================================================
void frame::do_set_lowlight_attribute(
    terminalpp::attribute const &lowlight_attribute)
{
    lowlight_attribute_ = lowlight_attribute;
    do_inner_focus_changed();
}

// ==========================================================================
// DO_SET_HIGHLIGHT_ATTRIBUTE
// ==========================================================================
void frame::do_set_highlight_attribute(
    terminalpp::attribute const &highlight_attribute)
{
    highlight_attribute_ = highlight_attribute;
    do_inner_focus_changed();
}

// ==========================================================================
// GET_FOCUS_ATTRIBUTE
// ==========================================================================
terminalpp::attribute frame::get_focus_attribute() const
{
    auto comp = inner_component_.lock();

    return comp && comp->has_focus() ? highlight_attribute_
                                     : lowlight_attribute_;
}

// ==========================================================================
// DO_CAN_RECEIVE_FOCUS
// ==========================================================================
bool frame::do_can_receive_focus() const
{
    return false;
}

}  // namespace munin