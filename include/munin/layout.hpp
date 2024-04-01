#pragma once

#include "munin/export.hpp"

#include <boost/any.hpp>
#include <nlohmann/json.hpp>
#include <terminalpp/extent.hpp>

#include <memory>
#include <vector>

namespace munin {

class component;

//* =========================================================================
/// \brief A class that knows how to lay components out in a container in
/// a specified manner.
//* =========================================================================
class MUNIN_EXPORT layout
{
public:
    //* =====================================================================
    /// \brief Destructor
    //* =====================================================================
    virtual ~layout() = default;

    //* =====================================================================
    /// \brief Returns the preferred size of this layout.
    /// \par
    /// The preferred size of the layout is the size that the layout would be
    /// if all components in it were at their preferred sizes.
    //* =====================================================================
    [[nodiscard]] terminalpp::extent get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any> const &hints) const;

    //* =====================================================================
    /// \brief Performs a layout of the specified components within the
    /// specified bounds.
    //* =====================================================================
    void operator()(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any> const &hints,
        terminalpp::extent size) const;

    //* =====================================================================
    /// \brief Returns details about the layout in JSON format.
    //* =====================================================================
    [[nodiscard]] nlohmann::json to_json() const;

protected:
    //* =====================================================================
    /// \brief Constructor
    //* =====================================================================
    layout() = default;

    //* =====================================================================
    /// \brief Called by get_preferred_size().  Derived classes must override
    /// this function in order to retrieve the preferred size of the layout
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] virtual terminalpp::extent do_get_preferred_size(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any> const &hints) const = 0;

    //* =====================================================================
    /// \brief Called by operator().  Derived classes must override this
    /// function in order to lay a container's components out in a custom
    /// manner.
    //* =====================================================================
    virtual void do_layout(
        std::vector<std::shared_ptr<component>> const &components,
        std::vector<boost::any> const &hints,
        terminalpp::extent size) const = 0;

    //* =====================================================================
    /// \brief Called by to_json().  Derived classes must override this
    /// function in order to add additional data about their implementation
    /// in a custom manner.
    //* =====================================================================
    [[nodiscard]] virtual nlohmann::json do_to_json() const = 0;
};

}  // namespace munin
