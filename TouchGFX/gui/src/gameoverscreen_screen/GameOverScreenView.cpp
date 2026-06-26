#include <gui/gameoverscreen_screen/GameOverScreenView.hpp>

#include <images/BitmapDatabase.hpp>

GameOverScreenView::GameOverScreenView()
{

}

touchgfx::BitmapId GameOverScreenView::getNumBitmap(uint8_t digit)
{
    const touchgfx::BitmapId numBitmaps[10] = {
        BITMAP_NUM_0_ID, BITMAP_NUM_1_ID, BITMAP_NUM_2_ID,
        BITMAP_NUM_3_ID, BITMAP_NUM_4_ID, BITMAP_NUM_5_ID,
        BITMAP_NUM_6_ID, BITMAP_NUM_7_ID, BITMAP_NUM_8_ID,
        BITMAP_NUM_9_ID
    };
    if (digit > 9) digit = 0;
    return numBitmaps[digit];
}

void GameOverScreenView::setScoreDisplay(int16_t score)
{
    if (score < 0) score = 0;
    if (score > 9999) score = 9999;

    uint8_t d[6];
    d[0] = score % 10;
    d[1] = (score / 10) % 10;
    d[2] = (score / 100) % 10;
    d[3] = (score / 1000) % 10;
    d[4] = (score / 10000) % 10;
    d[5] = (score / 100000) % 10;

    scoreD0.setBitmap(touchgfx::Bitmap(getNumBitmap(d[0]))); scoreD0.invalidate();
    scoreD1.setBitmap(touchgfx::Bitmap(getNumBitmap(d[1]))); scoreD1.invalidate();
    scoreD2.setBitmap(touchgfx::Bitmap(getNumBitmap(d[2]))); scoreD2.invalidate();
    scoreD3.setBitmap(touchgfx::Bitmap(getNumBitmap(d[3]))); scoreD3.invalidate();
    scoreD4.setBitmap(touchgfx::Bitmap(getNumBitmap(d[4]))); scoreD4.invalidate();
    scoreD5.setBitmap(touchgfx::Bitmap(getNumBitmap(d[5]))); scoreD5.invalidate();
}

void GameOverScreenView::setHighScoreDisplay(int16_t score)
{
    if (score < 0) score = 0;
    if (score > 9999) score = 9999;

    uint8_t d[6];
    d[0] = score % 10;
    d[1] = (score / 10) % 10;
    d[2] = (score / 100) % 10;
    d[3] = (score / 1000) % 10;
    d[4] = (score / 10000) % 10;
    d[5] = (score / 100000) % 10;

    hiD0.setBitmap(touchgfx::Bitmap(getNumBitmap(d[0]))); hiD0.invalidate();
    hiD1.setBitmap(touchgfx::Bitmap(getNumBitmap(d[1]))); hiD1.invalidate();
    hiD2.setBitmap(touchgfx::Bitmap(getNumBitmap(d[2]))); hiD2.invalidate();
    hiD3.setBitmap(touchgfx::Bitmap(getNumBitmap(d[3]))); hiD3.invalidate();
    hiD4.setBitmap(touchgfx::Bitmap(getNumBitmap(d[4]))); hiD4.invalidate();
    hiD5.setBitmap(touchgfx::Bitmap(getNumBitmap(d[5]))); hiD5.invalidate();
}

void GameOverScreenView::setupScreen()
{
    GameOverScreenViewBase::setupScreen();
    
    // Hien thi diem so hien tai va diem cao tu model
    setScoreDisplay(presenter->getScore());
    setHighScoreDisplay(presenter->getHighScore());
}

void GameOverScreenView::tearDownScreen()
{
    GameOverScreenViewBase::tearDownScreen();
}
