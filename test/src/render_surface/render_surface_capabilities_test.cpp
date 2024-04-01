#include <gtest/gtest.h>
#include <munin/render_surface.hpp>
#include <munin/render_surface_capabilities.hpp>

using testing::Values;

TEST(a_render_surface, supports_unicode_by_default)
{
    terminalpp::canvas cvs({1, 1});
    munin::render_surface rs(cvs);

    ASSERT_TRUE(rs.supports_unicode());
}

class render_surface_with_optional_unicode_capability
  : public testing::TestWithParam<bool>
{
};

TEST_P(
    render_surface_with_optional_unicode_capability,
    returns_that_capability_from_supports_unicode)
{
    auto const &unicode_support = GetParam();

    struct unicode_render_surface_capabilities
      : public munin::render_surface_capabilities
    {
        explicit unicode_render_surface_capabilities(bool unicode_support)
          : unicode_support_(unicode_support)
        {
        }

        [[nodiscard]] bool supports_unicode() const override
        {
            return unicode_support_;
        }

        bool unicode_support_;
    };

    terminalpp::canvas cvs({1, 1});
    unicode_render_surface_capabilities caps(unicode_support);
    munin::render_surface rs(cvs, caps);

    ASSERT_EQ(unicode_support, rs.supports_unicode());
}

INSTANTIATE_TEST_SUITE_P(
    render_surfaces_reflect_unicode_support_from_capabilities,
    render_surface_with_optional_unicode_capability,
    Values(true, false));
