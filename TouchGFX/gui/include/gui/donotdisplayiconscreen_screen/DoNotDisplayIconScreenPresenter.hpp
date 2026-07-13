#ifndef DONOTDISPLAYICONSCREENPRESENTER_HPP
#define DONOTDISPLAYICONSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DoNotDisplayIconScreenView;

class DoNotDisplayIconScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DoNotDisplayIconScreenPresenter(DoNotDisplayIconScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~DoNotDisplayIconScreenPresenter() {}

private:
    DoNotDisplayIconScreenPresenter();

    DoNotDisplayIconScreenView& view;
};

#endif // DONOTDISPLAYICONSCREENPRESENTER_HPP
