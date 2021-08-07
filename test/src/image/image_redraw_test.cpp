#include <munin/image.hpp>
#include <terminalpp/canvas.hpp>
#include <gtest/gtest.h>
#include <utility>

class an_image_to_be_redrawn : public testing::Test
{
protected :
    an_image_to_be_redrawn()
      : an_image_to_be_redrawn("")
    {
    }

    template <class T>
    explicit an_image_to_be_redrawn(T &&t)
      : image_(std::forward<T>(t))
    {
        image_.set_size({6, 4});

        image_.on_preferred_size_changed.connect(
            [this]
            {
                ++preferred_size_changed_called_;
                preferred_size_ = image_.get_preferred_size();
            });

        image_.on_redraw.connect(
            [this](auto const &regions)
            {
                ++redraw_called_;
                redraw_regions_ = regions;
            });
    }

    munin::image image_;

    int preferred_size_changed_called_ = 0;
    terminalpp::extent preferred_size_;

    int redraw_called_ = 0;
    std::vector<terminalpp::rectangle> redraw_regions_;
};

TEST_F(an_image_to_be_redrawn, redraws_entire_image_when_setting_fill)
{
    image_.set_fill('?');

    ASSERT_EQ(0, preferred_size_changed_called_);
    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(1u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({0, 0}, {6, 4}), redraw_regions_[0]);
}

class an_image_with_empty_content : public an_image_to_be_redrawn
{
};

TEST_F(an_image_with_empty_content, redraws_nothing_when_setting_content_to_empty)
{
    image_.set_content();

    ASSERT_EQ(0, preferred_size_changed_called_);
    ASSERT_EQ(0, redraw_called_);
}

TEST_F(an_image_with_empty_content, redraws_new_image_area_when_setting_content_to_single_line)
{
    using namespace terminalpp::literals;
    image_.set_content("test"_ts);

    ASSERT_EQ(1, preferred_size_changed_called_);
    ASSERT_EQ(terminalpp::extent(4, 1), preferred_size_);

    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(1u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 1}), redraw_regions_[0]);
}

TEST_F(an_image_with_empty_content, redraws_new_image_area_when_setting_content_to_multi_line)
{
    using namespace terminalpp::literals;
    std::vector<terminalpp::string> const content = {
        "test",
        "test"
    };

    image_.set_content(content);

    ASSERT_EQ(1, preferred_size_changed_called_);
    ASSERT_EQ(terminalpp::extent(4, 2), preferred_size_);

    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(1u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 2}), redraw_regions_[0]);
}

class an_image_with_single_line_content : public an_image_to_be_redrawn
{
protected :
    an_image_with_single_line_content()
      : an_image_to_be_redrawn("test")
    {
    }
};

TEST_F(an_image_with_single_line_content, redraws_old_image_area_when_setting_content_to_empty)
{
    image_.set_content();

    ASSERT_EQ(1, preferred_size_changed_called_);
    ASSERT_EQ(terminalpp::extent(0, 0), preferred_size_);

    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(1u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 1}), redraw_regions_[0]);
}

TEST_F(an_image_with_single_line_content, redraws_old_and_new_image_area_when_setting_content_to_larger_content)
{
    image_.set_content("xtestx");

    ASSERT_EQ(1, preferred_size_changed_called_);
    ASSERT_EQ(terminalpp::extent(6, 1), preferred_size_);

    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(2u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 1}), redraw_regions_[0]);
    ASSERT_EQ(terminalpp::rectangle({0, 1}, {6, 1}), redraw_regions_[1]);
}

TEST_F(an_image_with_single_line_content, redraws_old_area_and_new_area_when_setting_content_to_multi_line_content)
{
    std::vector<terminalpp::string> content = {
        "test",
        "test"
    };

    image_.set_content(content);

    ASSERT_EQ(1, preferred_size_changed_called_);
    ASSERT_EQ(terminalpp::extent(4, 2), preferred_size_);

    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(2u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 1}), redraw_regions_[0]);
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 2}), redraw_regions_[1]);
}

class an_image_with_multi_line_content : public an_image_to_be_redrawn
{
protected :
    an_image_with_multi_line_content()
      : an_image_to_be_redrawn(std::vector<terminalpp::string>{
            "test",
            "test"
        })
    {
    }
};

TEST_F(an_image_with_multi_line_content, redraws_old_area_when_setting_content_empty)
{
    image_.set_content();

    ASSERT_EQ(1, preferred_size_changed_called_);
    ASSERT_EQ(terminalpp::extent(0, 0), preferred_size_);

    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(1u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 2}), redraw_regions_[0]);
}

TEST_F(an_image_with_multi_line_content, redraws_old_and_new_area_when_setting_content_to_single_line)
{
    image_.set_content("test");

    ASSERT_EQ(1, preferred_size_changed_called_);
    ASSERT_EQ(terminalpp::extent(4, 1), preferred_size_);

    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(2u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 2}), redraw_regions_[0]);
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 1}), redraw_regions_[1]);
}

TEST_F(an_image_with_multi_line_content, redraws_old_and_new_area_when_setting_content_to_multi_line)
{
    std::vector<terminalpp::string> content {
        "xtestx",
        "xtestx"
    };
    image_.set_content(content);

    ASSERT_EQ(1, preferred_size_changed_called_);
    ASSERT_EQ(terminalpp::extent(6, 2), preferred_size_);

    ASSERT_EQ(1, redraw_called_);
    ASSERT_EQ(2u, redraw_regions_.size());
    ASSERT_EQ(terminalpp::rectangle({1, 1}, {4, 2}), redraw_regions_[0]);
    ASSERT_EQ(terminalpp::rectangle({0, 1}, {6, 2}), redraw_regions_[1]);
}
