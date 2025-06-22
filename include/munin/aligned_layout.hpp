#pragma once

#include "munin/layout.hpp"

namespace munin {

enum class horizontal_alignment : std::uint8_t
{
    left,
    centre,
    right,
    fill,
};

enum class vertical_alignment : std::uint8_t
{
    top,
    centre,
    bottom,
    fill,
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
    vertical_alignment vertical;
};

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// a aligned manner.
//* =========================================================================
class MUNIN_EXPORT aligned_layout final : public layout
{
protected:
    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<std::any> const &hints) const override;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    void do_layout(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<std::any> const &hints,
        terminalpp::extent size) const override;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] nlohmann::json do_to_json() const override;
};

//* =========================================================================
/// \brief Returns a newly created aligned layout
//* =========================================================================
MUNIN_EXPORT
std::unique_ptr<layout> make_aligned_layout();

// Now a selection of alignment constants that you can use.

inline constexpr alignment const alignment_hlvt = {
    .horizontal = horizontal_alignment::left,
    .vertical = vertical_alignment::top};

inline constexpr alignment const alignment_hcvt = {
    .horizontal = horizontal_alignment::centre,
    .vertical = vertical_alignment::top};

inline constexpr alignment const alignment_hrvt = {
    .horizontal = horizontal_alignment::right,
    .vertical = vertical_alignment::top};

inline constexpr alignment const alignment_hlvc = {
    .horizontal = horizontal_alignment::left,
    .vertical = vertical_alignment::centre};

inline constexpr alignment const alignment_hcvc = {
    .horizontal = horizontal_alignment::centre,
    .vertical = vertical_alignment::centre};

inline constexpr alignment const alignment_hrvc = {
    .horizontal = horizontal_alignment::right,
    .vertical = vertical_alignment::centre};

inline constexpr alignment const alignment_hlvb = {
    .horizontal = horizontal_alignment::left,
    .vertical = vertical_alignment::bottom};

inline constexpr alignment const alignment_hcvb = {
    .horizontal = horizontal_alignment::centre,
    .vertical = vertical_alignment::bottom};

inline constexpr alignment const alignment_hrvb = {
    .horizontal = horizontal_alignment::right,
    .vertical = vertical_alignment::bottom};

inline constexpr alignment const alignment_fill = {
    .horizontal = horizontal_alignment::fill,
    .vertical = vertical_alignment::fill};

}  // namespace munin
