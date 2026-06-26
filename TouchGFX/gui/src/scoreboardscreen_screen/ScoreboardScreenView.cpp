#include <gui/scoreboardscreen_screen/ScoreboardScreenView.hpp>

#include <images/BitmapDatabase.hpp>

ScoreboardScreenView::ScoreboardScreenView()
{

}

touchgfx::BitmapId ScoreboardScreenView::getNumBitmap(uint8_t digit)
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

void ScoreboardScreenView::setRankScoreDisplay(uint8_t rank, int16_t score)
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

    if (rank == 1)
    {
        r1D0.setBitmap(touchgfx::Bitmap(getNumBitmap(d[0]))); r1D0.invalidate();
        r1D1.setBitmap(touchgfx::Bitmap(getNumBitmap(d[1]))); r1D1.invalidate();
        r1D2.setBitmap(touchgfx::Bitmap(getNumBitmap(d[2]))); r1D2.invalidate();
        r1D3.setBitmap(touchgfx::Bitmap(getNumBitmap(d[3]))); r1D3.invalidate();
        r1D4.setBitmap(touchgfx::Bitmap(getNumBitmap(d[4]))); r1D4.invalidate();
        r1D5.setBitmap(touchgfx::Bitmap(getNumBitmap(d[5]))); r1D5.invalidate();
    }
    else if (rank == 2)
    {
        r2D0.setBitmap(touchgfx::Bitmap(getNumBitmap(d[0]))); r2D0.invalidate();
        r2D1.setBitmap(touchgfx::Bitmap(getNumBitmap(d[1]))); r2D1.invalidate();
        r2D2.setBitmap(touchgfx::Bitmap(getNumBitmap(d[2]))); r2D2.invalidate();
        r2D3.setBitmap(touchgfx::Bitmap(getNumBitmap(d[3]))); r2D3.invalidate();
        r2D4.setBitmap(touchgfx::Bitmap(getNumBitmap(d[4]))); r2D4.invalidate();
        r2D5.setBitmap(touchgfx::Bitmap(getNumBitmap(d[5]))); r2D5.invalidate();
    }
    else if (rank == 3)
    {
        r3D0.setBitmap(touchgfx::Bitmap(getNumBitmap(d[0]))); r3D0.invalidate();
        r3D1.setBitmap(touchgfx::Bitmap(getNumBitmap(d[1]))); r3D1.invalidate();
        r3D2.setBitmap(touchgfx::Bitmap(getNumBitmap(d[2]))); r3D2.invalidate();
        r3D3.setBitmap(touchgfx::Bitmap(getNumBitmap(d[3]))); r3D3.invalidate();
        r3D4.setBitmap(touchgfx::Bitmap(getNumBitmap(d[4]))); r3D4.invalidate();
        r3D5.setBitmap(touchgfx::Bitmap(getNumBitmap(d[5]))); r3D5.invalidate();
    }
}

void ScoreboardScreenView::setupScreen()
{
    ScoreboardScreenViewBase::setupScreen();
    
    // Dong bo diem Top 3 tu Presenter de hien thi len các dòng
    setRankScoreDisplay(1, presenter->getHighScoreAt(0));
    setRankScoreDisplay(2, presenter->getHighScoreAt(1));
    setRankScoreDisplay(3, presenter->getHighScoreAt(2));
}

void ScoreboardScreenView::tearDownScreen()
{
    ScoreboardScreenViewBase::tearDownScreen();
}
