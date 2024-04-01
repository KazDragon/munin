#include <gtest/gtest.h>
#include <munin/text_area.hpp>
#include <terminalpp/canvas.hpp>

class a_text_area_base
{
protected:
    static terminalpp::element const fill;

    munin::text_area text_area_;
    terminalpp::canvas canvas_{
        {0, 0}
    };

    void fill_canvas(terminalpp::extent canvas_size);
};

class a_text_area : public a_text_area_base, public testing::Test
{
};
