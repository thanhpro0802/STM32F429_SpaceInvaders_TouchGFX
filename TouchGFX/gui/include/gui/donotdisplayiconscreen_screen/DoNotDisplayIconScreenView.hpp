#ifndef DONOTDISPLAYICONSCREENVIEW_HPP
#define DONOTDISPLAYICONSCREENVIEW_HPP

#include <gui_generated/donotdisplayiconscreen_screen/DoNotDisplayIconScreenViewBase.hpp>
#include <gui/donotdisplayiconscreen_screen/DoNotDisplayIconScreenPresenter.hpp>

class DoNotDisplayIconScreenView : public DoNotDisplayIconScreenViewBase
{
public:
    DoNotDisplayIconScreenView();
    virtual ~DoNotDisplayIconScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DONOTDISPLAYICONSCREENVIEW_HPP
