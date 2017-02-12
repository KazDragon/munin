#pragma once

#include "munin/filled_box.hpp"

namespace munin {

//* =========================================================================
/// \brief Returns a component that is filled with background elements.
//* =========================================================================
MUNIN_EXPORT    
std::shared_ptr<filled_box> make_background_fill();

}
