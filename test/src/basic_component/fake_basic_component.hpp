#pragma once
#include <munin/basic_component.hpp>

// Tests run on this fake basic component that just completes the remaining
// virtual functions.
class fake_basic_component : public munin::basic_component
{
 private:
  [[nodiscard]] terminalpp::extent do_get_preferred_size() const override
  {
    return {0, 0};
  }

  void do_draw(
      munin::render_surface &surface,
      terminalpp::rectangle const &region) const override
  {
  }
};

class fake_basic_component_that_receives_focus : public fake_basic_component
{
};

class fake_basic_component_that_does_not_receive_focus
  : public fake_basic_component
{
 private:
  //* =====================================================================
  /// \brief Returns true if it is allowed for the component to receive
  /// focus, false otherwise.  This is used by the focus functions of
  /// basic_component.  By default, a component may receive focus at all
  /// times.  Override this function to specify different behaviour in your
  /// component.
  //* =====================================================================
  [[nodiscard]] bool do_can_receive_focus() const override
  {
    return false;
  }
};
