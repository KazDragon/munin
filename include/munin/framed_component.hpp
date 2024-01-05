#pragma once

#include "munin/composite_component.hpp"
#include "munin/frame.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that models a box that is always completely filled with
/// a given element.
//* =========================================================================
class MUNIN_EXPORT framed_component : public munin::composite_component
{
 public:
  //* =====================================================================
  /// \brief Constructor
  //* =====================================================================
  framed_component(
      std::shared_ptr<frame> outer_frame,
      std::shared_ptr<component> inner_component);

 protected:
  //* =====================================================================
  /// \brief Called by event().  Derived classes must override this
  /// function in order to handle events in a custom manner.
  //* =====================================================================
  void do_event(boost::any const &ev) override;

  //* =====================================================================
  /// \brief Called by to_json().  Derived classes must override this
  /// function in order to add additional data about their implementation
  /// in a custom manner.
  //* =====================================================================
  [[nodiscard]] nlohmann::json do_to_json() const override;

 private:
  std::shared_ptr<frame> frame_;
  std::shared_ptr<component> inner_component_;
};

//* =========================================================================
/// \brief Returns a newly created filled box.
//* =========================================================================
MUNIN_EXPORT
std::shared_ptr<framed_component> make_framed_component(
    std::shared_ptr<frame> outer_frame,
    std::shared_ptr<component> inner_component);

}  // namespace munin
