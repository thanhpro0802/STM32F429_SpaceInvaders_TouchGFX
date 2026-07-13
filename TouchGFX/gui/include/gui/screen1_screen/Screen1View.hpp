#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <gui/model/Model.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <stdint.h>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;
    virtual void handleKeyEvent(uint8_t key) override;
    void updateGameState(const GameState& state);
    void gotoGameOverScreen();
protected:
private:
    void setScoreDisplay(int32_t score);
    void setLivesDisplay(int8_t lives);
    void updateLevelIntro(const GameState& state);
    touchgfx::BitmapId getNumBitmap(uint8_t digit);
    touchgfx::BitmapId getEnemyBitmap(uint8_t type);

    touchgfx::Image* enemyImages[MAX_ENEMIES];
    touchgfx::TextureMapper enemyBullets[20];
    touchgfx::Image playerBulletLeft;
    touchgfx::Image playerBulletRight;
    touchgfx::Image playerBullet2;
    touchgfx::Image bossImage;
    touchgfx::Image bossForcefield;
    touchgfx::Box bossHpBarBg;
    touchgfx::Box bossHpBar;
    touchgfx::Image levelTensDigit;
    touchgfx::Image levelOnesDigit;

    int32_t lastScore;
    int8_t  lastLives;
    uint8_t lastLevel;
};

#endif // SCREEN1VIEW_HPP

