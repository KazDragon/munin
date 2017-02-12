#include "munin/background_fill.hpp"
#include "munin/filled_box.hpp"

namespace munin {

// ==========================================================================
// MAKE_BACKGROUND_FILL
// ==========================================================================
std::shared_ptr<filled_box> make_background_fill()
{
    return make_fill(' ');
}

}
