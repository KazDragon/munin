#pragma once

#include "munin/layout.hpp"

namespace munin {

enum class horizontal_alignment
{
    left,
    centre,
    right
};

enum class vertical_alignment
{
    top,
    centre,
    bottom
};

//* =========================================================================
/// \brief A hint for an aligned_layout about where to place the component.
//* =========================================================================
struct alignment
{
    //* =====================================================================
    /// \brief Where to place the component horizontally 
    /// (left, centre, right)
    //* =====================================================================
    horizontal_alignment horizontal;

    //* =====================================================================
    /// \brief Where to place the component vertically
    /// (top, centre, bottom)
    //* =====================================================================
    vertical_alignment   vertical;
};

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// a aligned manner.
//* =========================================================================
class MUNIN_EXPORT aligned_layout final
  : public layout
{
protected :
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any>                 const &hints) const override;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    void do_layout(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any>                 const &hints,
        terminalpp::extent                             size) const override;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    nlohmann::json do_to_json() const override;
};

//* =========================================================================
/// \brief Returns a newly created aligned layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_aligned_layout();

// Now a selection of alignment constants that you can use.

static constexpr alignment const alignment_hlvt = {
    horizontal_alignment::left,
    vertical_alignment::top
};

static constexpr alignment const alignment_hcvt = {
    horizontal_alignment::centre,
    vertical_alignment::top
};

static constexpr alignment const alignment_hrvt = {
    horizontal_alignment::right,
    vertical_alignment::top
};

static constexpr alignment const alignment_hlvc = {
    horizontal_alignment::left,
    vertical_alignment::centre
};

static constexpr alignment const alignment_hcvc = {
    horizontal_alignment::centre,
    vertical_alignment::centre
};

static constexpr alignment const alignment_hrvc = {
    horizontal_alignment::right,
    vertical_alignment::centre
};

static constexpr alignment const alignment_hlvb = {
    horizontal_alignment::left,
    vertical_alignment::bottom
};

static constexpr alignment const alignment_hcvb = {
    horizontal_alignment::centre,
    vertical_alignment::bottom
};

static constexpr alignment const alignment_hrvb = {
    horizontal_alignment::right,
    vertical_alignment::bottom
};

}
