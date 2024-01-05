#include "fake/channel.hpp"
#include "mock/component.hpp"
#include "mock/render_surface_capabilities.hpp"
#include <munin/window.hpp>
#include <boost/make_unique.hpp>
#include <gtest/gtest.h>
#include <optional>

using testing::_;
using testing::Return;
using testing::WithArg;

namespace {

class a_window_that_does_not_support_unicode : public testing::Test
{
 public:
  a_window_that_does_not_support_unicode()
  {
    ON_CALL(capabilities_, supports_unicode()).WillByDefault(Return(false));

    window_ =
        boost::make_unique<munin::window>(terminal_, content_, capabilities_);
  }

 protected:
  mock_render_surface_capabilities capabilities_;
  std::shared_ptr<mock_component> content_{make_mock_component()};

  fake_channel channel_;
  terminalpp::terminal terminal_{channel_};
  std::unique_ptr<munin::window> window_;
};

}  // namespace

TEST_F(
    a_window_that_does_not_support_unicode,
    passes_those_capabilities_to_components)
{
  std::optional<bool> supports_unicode = std::nullopt;
  ON_CALL(*content_, do_draw(_, _))
      .WillByDefault(
          WithArg<0>([&supports_unicode](munin::render_surface &surface)
                     { supports_unicode = surface.supports_unicode(); }));

  terminalpp::canvas cvs({3, 3});
  window_->repaint(cvs);

  ASSERT_TRUE(supports_unicode.has_value());
  ASSERT_FALSE(*supports_unicode);
}