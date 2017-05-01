#include "munin/tabbed_panel.hpp"
#include "munin/card.hpp"
#include "munin/container.hpp"
#include "munin/framed_component.hpp"
#include "munin/grid_layout.hpp"
#include "munin/tabbed_frame.hpp"

namespace munin {

// ==========================================================================
// TABBED_PANEL::IMPLEMENTATION STRUCTURE
// ==========================================================================
struct tabbed_panel::impl
{
    std::shared_ptr<card>         card_;
    std::shared_ptr<tabbed_frame> frame_;

    // ======================================================================
    // TAB_SELECTED
    // ======================================================================
    void tab_selected(std::string const &text)
    {
        card_->select_face(text);
    }

    // ======================================================================
    // FOCUS_CHANGE
    // ======================================================================
    void focus_change()
    {
        frame_->set_highlight(card_->has_focus() || frame_->has_focus());
    }
};

// ==========================================================================
// CONSTRUCTOR
// ==========================================================================
tabbed_panel::tabbed_panel()
    : pimpl_(new impl)
{
    pimpl_->card_  = make_card();
    pimpl_->frame_ = make_tabbed_frame();

    auto inner = make_framed_component(pimpl_->frame_, pimpl_->card_);

    auto content = get_container();
    content->set_layout(make_grid_layout(1, 1));
    content->add_component(inner);

    pimpl_->frame_->on_tab_selected.connect(
        [wpthis=std::weak_ptr<impl>(pimpl_)]
        (auto idx)
        {
            auto pthis = wpthis.lock();

            if (pthis)
            {
                pthis->tab_selected(idx);
            }
        });

    auto focus_callback =
        [wpthis=std::weak_ptr<impl>(pimpl_)]
        {
            auto pthis = wpthis.lock();

            if (pthis)
            {
                pthis->focus_change();
            }
        };

    pimpl_->frame_->on_focus_set.connect(focus_callback);
    pimpl_->frame_->on_focus_lost.connect(focus_callback);
    pimpl_->card_->on_focus_set.connect(focus_callback);
    pimpl_->card_->on_focus_lost.connect(focus_callback);
}

// ==========================================================================
// DESTRUCTOR
// ==========================================================================
tabbed_panel::~tabbed_panel()
{
}

// ==========================================================================
// INSERT_TAB
// ==========================================================================
void tabbed_panel::insert_tab(
    std::string                const &text,
    std::shared_ptr<component> const &comp,
    boost::optional<odin::u32>        index /*= optional<u32>()*/)
{
    pimpl_->card_->add_face(comp, text);
    pimpl_->frame_->insert_tab(text, index);

    if (pimpl_->card_->get_number_of_faces() == 1)
    {
        pimpl_->card_->select_face(text);
    }
}

// ==========================================================================
// REMOVE_TAB
// ==========================================================================
void tabbed_panel::remove_tab(odin::u32 index)
{
    pimpl_->frame_->remove_tab(index);
}

// ==========================================================================
// MAKE_TABBED_PANEL
// ==========================================================================
std::shared_ptr<tabbed_panel> make_tabbed_panel()
{
    return std::make_shared<tabbed_panel>();
}

}
