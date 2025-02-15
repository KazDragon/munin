#pragma once

#include "munin/container.hpp"
#include "munin/layout.hpp"

#include <any>
#include <memory>
#include <type_traits>
#include <utility>

namespace munin {
namespace detail {

// Overload where the next argument is convertible to a component.
template <class... Args>
void view_helper(
    std::shared_ptr<container> const &content,
    std::shared_ptr<component> const &prev,
    std::shared_ptr<component> const &comp,
    Args &&...args);

// Overload where the next argument is not convertible to a component.
// In this case, the argument is automatically wrapped in a Boost.Any
// object and passed as the layout hint.
template <
    class Hint,
    class = std::enable_if_t<
        !std::is_convertible_v<Hint, std::shared_ptr<component>>>,
    class... Args>
void view_helper(
    std::shared_ptr<container> const &content,
    std::shared_ptr<component> const &prev,
    Hint &&hint,
    Args &&...args);

// Overload where there is no next argument.  This is the terminal case.
inline void view_helper(
    std::shared_ptr<container> const &content,
    std::shared_ptr<component> const &prev)
{
    if (prev)
    {
        content->add_component(prev);
    }
}

}  // namespace detail

//* =========================================================================
/// \brief Constructs a container with a given layout and components.
/// \par Usage
/// Simply call, passing a layout and a series of components and/or
/// layout hints.
/// \par Example #1
/// \code
///     auto v = munin::view(
///         munin::make_compass_layout(),
///         // Note: passing both components and layout hints.
///         munin::make_image("Name: "),
///         munin::compass_layout::heading::west,
///         munin::make_fill(' '),
///         munin::compass_layout::heading::centre);
/// \endcode
/// \par Example #2
/// \code
///     auto y = munin::view(
///         munin::make_vertical_strip_layout(),
///         // Note: passing components only; no hints.
///         munin::make_button("OK"),
///         munin::make_button("Cancel"));
/// \endcode
//* =========================================================================
template <class... Args>
std::shared_ptr<container> view(std::unique_ptr<layout> lyt, Args &&...args)
{
    auto comp = munin::make_container();
    comp->set_layout(std::move(lyt));
    detail::view_helper(
        comp, std::shared_ptr<component>{}, std::forward<Args>(args)...);

    return comp;
}

namespace detail {

template <class... Args>
void view_helper(
    std::shared_ptr<container> const &content,
    std::shared_ptr<component> const &prev,
    std::shared_ptr<component> const &comp,
    Args &&...args)
{
    if (prev)
    {
        content->add_component(prev);
    }

    view_helper(content, comp, std::forward<Args>(args)...);
}

template <class Hint, class, class... Args>
void view_helper(
    std::shared_ptr<container> const &content,
    std::shared_ptr<component> const &prev,
    Hint &&hint,
    Args &&...args)
{
    if (prev)
    {
        content->add_component(prev, std::any(std::forward<Hint>(hint)));
    }

    view_helper(content, {}, std::forward<Args>(args)...);
}

}  // namespace detail

}  // namespace munin
