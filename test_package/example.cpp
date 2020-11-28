#include <munin/window.hpp>
#include <munin/null_layout.hpp>
#include <munin/view.hpp>
#include <cstdlib>

int main() {
    munin::window w{munin::view(munin::make_null_layout())};
    return EXIT_SUCCESS;
}
