#include "fill_canvas.hpp"
#include <terminalpp/algorithm/for_each_in_region.hpp>

void fill_canvas(terminalpp::canvas &canvas, terminalpp::element const &fill)
{
    terminalpp::for_each_in_region(
        canvas,
        {{}, canvas.size()},
        [fill](terminalpp::element &elem,
               terminalpp::coordinate_type column,
               terminalpp::coordinate_type row)
        {
            elem = fill;
        });
}
