#pragma once

#include "munin/layout.hpp"

namespace munin {

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in the
/// manner of a compass, pinning components to the edge associated with the
/// heading.  Each component is given its preferred dimension away from that
/// edge and fills the dimension along the edge.  A special heading of
/// "centre" fills the remaining space.
///
/// Note: this can be sensitive to insertion order.
///
/// \verbatim
/// +------------+ +---+-----+---+
/// |   NORTH    | | W |  C  | E |
/// +------------+ | E |  E  | A |
/// |            | | S |  N  | S |
/// |   CENTRE   | | T |  T  | T |
/// |            | |   |  R  |   |
/// |            | |   |  E  |   |
/// +------------+ +---+-----+---+
/// \endverbatim
///
//* =========================================================================
class MUNIN_EXPORT compass_layout final : public layout
{
public:
    //* =====================================================================
    /// \brief An enumeration of possible compass headings within the
    /// layout.
    //* =====================================================================
    enum class heading
    {
        centre,
        north,
        south,
        east,
        west
    };

protected:
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any> const &hints) const override;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    void do_layout(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any> const &hints,
        terminalpp::extent size) const override;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] nlohmann::json do_to_json() const override;
};

//* =========================================================================
/// \brief Returns a newly created compass layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_compass_layout();

}  // namespace munin
