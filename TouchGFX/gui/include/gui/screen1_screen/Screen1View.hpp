#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <gui/model/Model.hpp>
#include <touchgfx/widgets/Image.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleKeyEvent(uint8_t key) override;
    void updateGameState(const GameState& state);
    void gotoGameOverScreen();
protected:
private:
    void setScoreDisplay(int16_t score);
    void setLivesDisplay(int8_t lives);
    touchgfx::BitmapId getNumBitmap(uint8_t digit);

    touchgfx::Image* enemyImages[MAX_ENEMIES];

    int16_t lastScore;
    int8_t  lastLives;
};

#endif // SCREEN1VIEW_HPP

