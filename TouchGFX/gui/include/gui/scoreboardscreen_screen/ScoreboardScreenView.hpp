#ifndef SCOREBOARDSCREENVIEW_HPP
#define SCOREBOARDSCREENVIEW_HPP

#include <gui_generated/scoreboardscreen_screen/ScoreboardScreenViewBase.hpp>
#include <gui/scoreboardscreen_screen/ScoreboardScreenPresenter.hpp>

class ScoreboardScreenView : public ScoreboardScreenViewBase
{
public:
    ScoreboardScreenView();
    virtual ~ScoreboardScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
private:
    void setRankScoreDisplay(uint8_t rank, int16_t score);
    touchgfx::BitmapId getNumBitmap(uint8_t digit);
};

#endif // SCOREBOARDSCREENVIEW_HPP
