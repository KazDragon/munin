#pragma once
#include <munin/container.hpp>
#include <gtest/gtest.h>

class container_test_base : public testing::Test
{
protected :
    void SetUp() override
    {
        container_.on_preferred_size_changed.connect(
            [this]()
            {
                ++preferred_size_changed_count;
            });

        container_.on_focus_set.connect(
            [this]()
            {
                ++focus_set_count;
            });

        container_.on_focus_lost.connect(
            [this]()
            {
                ++focus_lost_count;
            });
    }

    munin::container container_;

    int preferred_size_changed_count = 0;
    int focus_set_count = 0;
    int focus_lost_count = 0;
};

class a_container : public container_test_base
{
};

class a_new_container : public container_test_base
{

};

