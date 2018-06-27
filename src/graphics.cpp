#include "munin/graphics.hpp"
#include <cassert>

namespace munin {
namespace {

// ==========================================================================
// Default Graphics structure.  Provides a baseline set of capabilities
// that can be rendered universally.
// ==========================================================================
class default_graphics : public graphics
{
public :
    bool supports_unicode() const override
    {
        return false;
    }
};

// ==========================================================================
// A default graphics object.  This also acts as a null object for when
// no specific graphics object is set.
// ==========================================================================
default_graphics default_gr;

// ==========================================================================
// A graphics object that is local to the current thread.  This allows
// graphics objects to be set for independent components that are being 
// handled concurrently.
// ==========================================================================
thread_local graphics *current_thread_graphics = &default_gr;

}

// ==========================================================================
// GET_GRAPHICS
// ==========================================================================
graphics &get_graphics()
{
    assert(current_thread_graphics != nullptr);
    return *current_thread_graphics;
}

// ==========================================================================
// SET_GRAPHICS
// ==========================================================================
void set_graphics(graphics *gr)
{
    current_thread_graphics = gr == nullptr
                            ? &default_gr
                            : gr;
}

}