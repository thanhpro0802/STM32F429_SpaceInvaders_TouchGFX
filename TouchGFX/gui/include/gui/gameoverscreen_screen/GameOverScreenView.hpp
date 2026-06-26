#ifndef GAMEOVERSCREENVIEW_HPP
#define GAMEOVERSCREENVIEW_HPP

#include <gui_generated/gameoverscreen_screen/GameOverScreenViewBase.hpp>
#include <gui/gameoverscreen_screen/GameOverScreenPresenter.hpp>

class GameOverScreenView : public GameOverScreenViewBase
{
public:
    GameOverScreenView();
    virtual ~GameOverScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
private:
    void setScoreDisplay(int16_t score);
    void setHighScoreDisplay(int16_t score);
    touchgfx::BitmapId getNumBitmap(uint8_t digit);
};

#endif // GAMEOVERSCREENVIEW_HPP
